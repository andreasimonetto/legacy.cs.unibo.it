/* mre.cc - Render Engine implementation
Copyright (C) 2008  Andrea Simonetto

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "mre/alpha.h"
#include "mre/image.h"
#include "mre/Camera"
#include "mre/Cube"
#include "mre/Light"
#include "mre/Material"
#include "mre/Mesh"
#include "mre/Object"
#include "mre/Plane"
#include "mre/RenderEngine"
#include "mre/SceneFactory"
#include "mre/SceneManager"
#include "mre/SkyBox"
#include "mre/Sphere"
#include "mre/Texture2D"
#include "sys/System"
#include "sys/Timer"

#include <dirent.h>
#include <fnmatch.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <map>
#include <string>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#define SNAPSHOT_DIR "data/snapshots/"

using namespace sys;
using namespace std;

namespace mre {

RenderEngine *RenderEngine::instance = 0;
unsigned RenderEngine::snapshotId = 0;
SceneFactory *SceneFactory::instance = 0;
SceneManager *SceneManager::instance = 0;

static int snapshot_dir_filter(const struct dirent *entry)
{
	return !fnmatch("snapshot???.ppm", entry->d_name, 0);
}

static void* mre_load_ppm(const char *path, unsigned *width, unsigned *height, unsigned *depth)
{
// Max line <= 70 in PPM (per spec).
char head[70 + 1];
FILE* fp = fopen(path, "rb");
void *data = 0;
int i;

	if(!fp) {
		perror("uplezz");
		fprintf(stderr, "uplezz: %s\n", path);
		//System::getInstance()->exit(1);
	}
    
	// Grab first two chars of the file and make sure that it has the
	// correct magic cookie for a raw PPM file.
	fgets(head, 70, fp);
	if(strncmp(head, "P6", 2)) {
		fprintf(stderr, "%s: Not a raw PPM file\n", path);
		//System::getInstance()->exit(1);
	}
   
	// Grab the three elements in the header (height, width, maxval)
	i = 0;
	while(i < 3) {
		fgets(head, 70, fp);
		// Skip comments
		if(head[0] != '#') { 
			if(i == 0)
				i += sscanf(head, "%d %d %d", width, height, depth);
			else if(i == 1)
				i += sscanf(head, "%d %d", height, depth);
			else if(i == 2)
				i += sscanf(head, "%d", depth);
		}
	}

	// Grab all the image data in one fell swoop
	data = malloc((*width) * (*height) * 3);
	fread(data, 1, (*width) * (*height) * 3, fp);
	fclose(fp);
	return data;
}

extern "C" { void glWindowPos2f(GLfloat, GLfloat); }

void RenderEngine::render(const image_t *image, unsigned x, unsigned y) const
{
#if 0
double mvmatrix[16], pjmatrix[16];
static int idViewport[4] = { -1, -1, 2, 2 };
double ex = (double)x / (0.5 * (double)width) - 1.0;
double ey = 1.0 - (double)(y + image->height) / (0.5 * (double)height);
double ox, oy, oz;

	// Use the inverse of the modelview matrix 
	// to transform (ex, ey, ex) from eye to object space
	glGetDoublev(GL_PROJECTION_MATRIX, pjmatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	gluUnProject(ex, ey, 0.125, mvmatrix, pjmatrix, idViewport, &ox, &oy, &oz);
	glRasterPos3d(ox, oy, oz);
	glDrawPixels(image->width, image->height, GL_RGBA, GL_UNSIGNED_BYTE, image->pixel_data);
#else
	glWindowPos2f(x, height - image->height - y);
	glDrawPixels(image->width, image->height, GL_RGBA, GL_UNSIGNED_BYTE, image->pixel_data);
#endif
}

void __mre_close(int status)
{
	if(SceneManager::instance)
		delete SceneManager::instance;
	if(SceneFactory::instance)
		delete SceneFactory::instance;
	if(RenderEngine::instance)
		delete RenderEngine::instance;
}

void Camera::activate() const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(position[0], position[1], position[2],
		center[0], center[1], center[2],
		viewup[0], viewup[1], viewup[2]);
}

void Cube::render() const 
{
float n = reverseNormals ? -1.0 : 1.0;

	material.bind();
	glBegin(GL_QUADS);

	glNormal3f(0.0, n * -1.0, 0.0);
	glVertex3f(-edge / 2.0, -edge / 2.0, -edge / 2.0);
	glVertex3f(n * edge / 2.0, -edge / 2.0, n * -edge / 2.0);
	glVertex3f(edge / 2.0, -edge / 2.0, edge / 2.0);
	glVertex3f(n * -edge / 2.0, -edge / 2.0, n * edge / 2.0);

	glNormal3f(0.0, n * 1.0, 0.0);
	glVertex3f(-edge / 2.0, edge / 2.0, -edge / 2.0);
	glVertex3f(n * -edge / 2.0, edge / 2.0, n * edge / 2.0);
	glVertex3f(edge / 2.0, edge / 2.0, edge / 2.0);
	glVertex3f(n * edge / 2.0, edge / 2.0, n * -edge / 2.0);

	glNormal3f(n * -1.0, 0.0, 0.0);
	glVertex3f(-edge / 2.0, -edge / 2.0, -edge / 2.0);
	glVertex3f(-edge / 2.0, n * -edge / 2.0, n * edge / 2.0);
	glVertex3f(-edge / 2.0, edge / 2.0, edge / 2.0);
	glVertex3f(-edge / 2.0, n * edge / 2.0, n * -edge / 2.0);

	glNormal3f(n * 1.0, 0.0, 0.0);
	glVertex3f(edge / 2.0, -edge / 2.0, -edge / 2.0);
	glVertex3f(edge / 2.0, n * edge / 2.0, n * -edge / 2.0);
	glVertex3f(edge / 2.0, edge / 2.0, edge / 2.0);
	glVertex3f(edge / 2.0, n * -edge / 2.0, n * edge / 2.0);

	glNormal3f(0.0, 0.0, n * -1.0);
	glVertex3f(-edge / 2.0, -edge / 2.0, -edge / 2.0);
	glVertex3f(n * -edge / 2.0, n * edge / 2.0, -edge / 2.0);
	glVertex3f(edge / 2.0, edge / 2.0, -edge / 2.0);
	glVertex3f(n * edge / 2.0, n * -edge / 2.0, -edge / 2.0);

	glNormal3f(0.0, 0.0, n * 1.0);
	glVertex3f(-edge / 2.0, -edge / 2.0, edge / 2.0);
	glVertex3f(n * edge / 2.0, n * -edge / 2.0, edge / 2.0);
	glVertex3f(edge / 2.0, edge / 2.0, edge / 2.0);
	glVertex3f(n * -edge / 2.0, n * edge / 2.0, edge / 2.0);

	glEnd();
	material.unbind();
}

void Light::activate(unsigned id) const
{
	glLightfv(GL_LIGHT0 + id, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0 + id, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0 + id, GL_POSITION, pos);
}

void Material::bind() const 
{
	if(mapKd)
		mapKd->bind();

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void Material::unbind() const 
{
	if(mapKd)
		mapKd->unbind();
}

void Mesh::render() const 
{
	Material defaultMtl, *curMtl;
	int nid, vid, tid;

	// glDisable(GL_COLOR_MATERIAL);
	for(map<string, Group>::const_iterator group = groups.begin(); group != groups.end(); group++) {
		curMtl = group->second.material ? group->second.material : &defaultMtl;
		curMtl->bind();
		glBegin(GL_TRIANGLES);
		for(unsigned i = 0; i < group->second.triangles.size(); i++)
		{
		Triangle *triangle = triangles[group->second.triangles[i]];
/*
		const Vector3f &v1 = vertices[triangle->vindices[0] < 0 ? vertices.size() + triangle->vindices[0] : triangle->vindices[0] - 1];
		const Vector3f &v2 = vertices[triangle->vindices[1] < 0 ? vertices.size() + triangle->vindices[1] : triangle->vindices[1] - 1];
		const Vector3f &v3 = vertices[triangle->vindices[2] < 0 ? vertices.size() + triangle->vindices[2] : triangle->vindices[2] - 1];
		Vector3f l1 = v1 - v2;
		Vector3f l2 = v1 - v3;
		Vector3f norm = l1 * l2;
			
			norm.normalize();
*/

			for(int j = 0; j < 3; j++) {
				nid = triangle->nindices[j] < 0 ? normals.size() + triangle->nindices[j] : triangle->nindices[j] - 1;
				tid = triangle->tindices[j] < 0 ? texcoords.size() + triangle->tindices[j] : triangle->tindices[j] - 1;
				vid = triangle->vindices[j] < 0 ? vertices.size() + triangle->vindices[j] : triangle->vindices[j] - 1;

				if(nid >= 0)
					glNormal3fv(*normals[nid]);

				if(tid >= 0)
					glTexCoord2fv(*texcoords[tid]);

				glVertex3fv(*vertices[vid]);
			}
		}
		glEnd();
		curMtl->unbind();
	}
}

void Object::rotate(float degree, Axis axis)
{
float ct, st, t1, t2;

	rot[axis] += degree;
	while(rot[axis] >= 360.0)
		rot[axis] -= 360.0;
	while(rot[axis] < 0.0)
		rot[axis] += 360.0;

	ct = cosf(degree * M_PI / 180.0);
	st = sinf(degree * M_PI / 180.0);

	switch(axis) {
		case AXIS_X:
			t1 = tm[1] * ct - tm[2] * st;
			t2 = tm[1] * st + tm[2] * ct;
			tm[1] = t1;
			tm[2] = t2;
			t1 = tm[5] * ct - tm[6] * st;
			t2 = tm[5] * st + tm[6] * ct;
			tm[5] = t1;
			tm[6] = t2;
			t1 = tm[9] * ct - tm[10] * st;
			t2 = tm[9] * st + tm[10] * ct;
			tm[9] = t1;
			tm[10] = t2;
			break;

		case AXIS_Y:
			t1 = tm[0] * ct + tm[2] * st;
			t2 = -tm[0] * st + tm[2] * ct;
			tm[0] = t1;
			tm[2] = t2;
			t1 = tm[4] * ct + tm[6] * st;
			t2 = -tm[4] * st + tm[6] * ct;
			tm[4] = t1;
			tm[6] = t2;
			t1 = tm[8] * ct + tm[10] * st;
			t2 = -tm[8] * st + tm[10] * ct;
			tm[8] = t1;
			tm[10] = t2;
			break;

		case AXIS_Z:
			t1 = tm[0] * ct - tm[1] * st;
			t2 = tm[0] * st + tm[1] * ct;
			tm[0] = t1;
			tm[1] = t2;
			t1 = tm[4] * ct - tm[5] * st;
			t2 = tm[4] * st + tm[5] * ct;
			tm[4] = t1;
			tm[5] = t2;
			t1 = tm[8] * ct - tm[9] * st;
			t2 = tm[8] * st + tm[9] * ct;
			tm[8] = t1;
			tm[9] = t2;
			break;
	}
}

void Object::setPosition(float x, float y, float z) 
{
	tm[12] = x;
	tm[13] = y;
	tm[14] = z;
/*
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;*/
}

void Object::move(float x, float y, float z) 
{
	tm[12] += x;
	tm[13] += y;
	tm[14] += z;

	pos[0] += x;
	pos[1] += y;
	pos[2] += z;
}

void Object::setScale(float xfact, float yfact, float zfact) 
{
	tm[0] *= xfact;
	tm[5] *= yfact;
	tm[10] *= zfact;

	scf[0] *= xfact;
	scf[1] *= yfact;
	scf[2] *= zfact;
}

void Object::transformBegin() const 
{
//	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
//	glLoadIdentity();
	glMultMatrixf(tm);
/*
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(rot[0], 1, 0, 0);
	glRotatef(rot[1], 0, 1, 0);
	glRotatef(rot[2], 0, 0, 1);
	glScalef(scf[0], scf[1], scf[2]);*/
}

void Object::transformEnd() const {
	glPopMatrix();
}

void Object::prerender()
{
	if(displayList)
		glDeleteLists(displayList, 1);

	displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE);
	this->render();
	glEndList();
}

void Plane::render() const
{
float n = (reverseNormals ? -1.0 : 1.0);

	material.bind();

	glBegin(GL_QUADS);
	glNormal3f(0.0, n * 1.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-width / 2.0, 0.0, -height / 2.0);
	if(reverseNormals)
		glTexCoord2f(1.0, 0.0);
	else
		glTexCoord2f(0.0, 1.0);
	glVertex3f(n * -width / 2.0, 0.0, n * height / 2.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(width / 2.0, 0.0, height / 2.0);
	if(reverseNormals)
		glTexCoord2f(0.0, 1.0);
	else
		glTexCoord2f(1.0, 0.0);
	glVertex3f(n * width / 2.0, 0.0, n * -height / 2.0);
	glEnd();

	material.unbind();
}

RenderEngine::RenderEngine()
{
struct dirent **namelist;
int n = scandir(SNAPSHOT_DIR, &namelist, snapshot_dir_filter, alphasort);

	if(n > 0) {
		//
		sscanf(namelist[n - 1]->d_name, "snapshot%d.ppm", &snapshotId);

		//
		while(n--) 
			free(namelist[n]);
		free(namelist);
	}

	// 
	fadingFact = fadingVal = 0.0;
	reshapeHandler = 0;
	keyPressHandler = 0;
	keyReleaseHandler = 0;
	mouseMotionHandler = 0;
	mouseButtonPressHandler = 0;
	mouseButtonReleaseHandler = 0;
}

RenderEngine::~RenderEngine()
{
	glXDestroyContext(dpy, cx);
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void RenderEngine::initMainWindow(int argc, char *argv[], const char *title, unsigned x, unsigned y, unsigned width, unsigned height)
{
Colormap cmap;
XSetWindowAttributes swa;
GLXFBConfig *fbConfig;
int dummy;
int snglBuf[] = { GLX_DOUBLEBUFFER, 0, GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR, GLX_X_RENDERABLE, 1, GLX_RENDER_TYPE, GLX_RGBA_BIT, 
//			GLX_ACCUM_RED_SIZE, 1, GLX_ACCUM_GREEN_SIZE, 1, GLX_ACCUM_BLUE_SIZE, 1, GLX_ACCUM_ALPHA_SIZE, 1,
			GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, GLX_ALPHA_SIZE, 1, GLX_DEPTH_SIZE, 1, 
			GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT, 0 };
int dblBuf[] =  { GLX_DOUBLEBUFFER, 1, GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR, GLX_X_RENDERABLE, 1, GLX_RENDER_TYPE, GLX_RGBA_BIT, 
//			GLX_ACCUM_RED_SIZE, 1, GLX_ACCUM_GREEN_SIZE, 1, GLX_ACCUM_BLUE_SIZE, 1, GLX_ACCUM_ALPHA_SIZE, 1,
			GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, GLX_ALPHA_SIZE, 1, GLX_DEPTH_SIZE, 1, 
			GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT, 0 };

	doubleBuffer = needReshape = true;
	needRedraw = false;

	if(!(dpy = XOpenDisplay(0)))
		System::getInstance()->fatalError("could not open display");

	if(!glXQueryExtension(dpy, &dummy, &dummy))
		System::getInstance()->fatalError("X server has no OpenGL GLX extension");

	if(!(fbConfig = glXChooseFBConfig(dpy, DefaultScreen(dpy), dblBuf, &dummy))) {
		if(!(fbConfig = glXChooseFBConfig(dpy, DefaultScreen(dpy), snglBuf, &dummy))) 
			System::getInstance()->fatalError("no RGB visual with depth buffer");
		doubleBuffer = false;
	}

	vi = glXGetVisualFromFBConfig(dpy, *fbConfig);
	if(vi->c_class != TrueColor)
		System::getInstance()->fatalError("TrueColor visual required for this program");

	/* No sharing of display lists, direct rendering if possible */
	cx = glXCreateNewContext(dpy, *fbConfig, GLX_RGBA_TYPE, None, true);

	if(!cx)
		System::getInstance()->fatalError("could not create rendering context");

	if(!glXIsDirect(dpy, cx))
		cout << "Warning: unable to create direct rendering context, this may hurt performance." << endl;

	this->width = width;
	this->height = height;

	cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.border_pixel = 0;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask;
	win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 
		      x, y, width, height, 0, vi->depth, InputOutput, 
		      vi->visual, CWBorderPixel | CWColormap | 
		      CWEventMask, &swa);

	XSetStandardProperties(dpy, win, title, title, None, argv, argc, 0);

	glXMakeCurrent(dpy, win, cx);
	XMapWindow(dpy, win);

	cout << "Starting " << (doubleBuffer ? "double" : "single") << " buffer mode." << endl;

	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glReadBuffer(doubleBuffer ? GL_BACK : GL_FRONT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	reshape();
}

void RenderEngine::reshape()
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)width / (float)height, 1.0, 1000.0);

	if(reshapeHandler)
		reshapeHandler(width, height);

	glMatrixMode(GL_MODELVIEW);
}

int RenderEngine::mainLoop()
{
XEvent event;
Timer redrawTimer(40000);
unsigned key;

	// Main loop 
	System::getInstance()->pushExitHandler(__mre_close);
	quit = false;
	while(!quit) { 
		if(!XEventsQueued(dpy, QueuedAfterFlush)) {
			SceneManager::getInstance()->run();
		}
		else {
			// Event dispatch branch 
			do {
				XNextEvent(dpy, &event);
			} while(XEventsQueued(dpy, QueuedAfterFlush) > 0 &&
				(event.type == MotionNotify || event.type == KeyPress || event.type == KeyRelease));

			switch(event.type) {
				case KeyPress:
					key = XLookupKeysym(&event.xkey, 0);
					if(keyPressHandler) {
						keyPressHandler(key, ((XKeyEvent*)&event)->state, event.xmotion.x, event.xmotion.y);
					}
					break;
				case KeyRelease:
					key = XLookupKeysym(&event.xkey, 0);
					if(keyReleaseHandler) {
						keyReleaseHandler(key, ((XKeyEvent*)&event)->state, event.xmotion.x, event.xmotion.y);
					}
					break;
				break;
				case MotionNotify:
					if(mouseMotionHandler)
						mouseMotionHandler(((XMotionEvent*)&event)->state, event.xmotion.x, event.xmotion.y);
					break;
				case ButtonPress:
					if(mouseButtonPressHandler)
						mouseButtonPressHandler(event.xbutton.button, ((XButtonEvent*)&event)->state, event.xmotion.x, event.xmotion.y);
					break;
				case ButtonRelease:
					if(mouseButtonReleaseHandler)
						mouseButtonReleaseHandler(event.xbutton.button, ((XButtonEvent*)&event)->state, event.xmotion.x, event.xmotion.y);
					break;
				case ConfigureNotify:
					width = event.xconfigure.width;
					height = event.xconfigure.height;

					// glViewport(0, 0, event.xconfigure.width, event.xconfigure.height); 

					needReshape = true;
					/* Fall through... */

				case Expose:
					needRedraw = true;
					break;
			}
		}

		if(needReshape) {
			reshape();
			needReshape = false;
			render();
		}

		if(needRedraw && redrawTimer.hasExpired()) {
			render();
			needRedraw = false;
			redrawTimer.restart();
		}
	}

	return 0;
}

bool RenderEngine::snapshot()
{
const int aaSamples = 21;
const float aaFactor = 0.015625;
unsigned char scanline[3 * width];
FILE *fp;
char path[128];
	
	//
	sprintf(path, "%s/snapshot%03d.ppm", SNAPSHOT_DIR, ++snapshotId);
	fp = fopen(path, "wb");
	if(!fp) {
		cout << "Unable to open snapshot file '" << path << "'." << endl;
		return false;
	}

	glClear(GL_ACCUM_BUFFER_BIT);
	cout << "Building snapshot with antialias";
	cout.flush();
	for(int i = -aaSamples / 2; i < aaSamples / 2 + 1; i++) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0 + aaFactor * i, (float)width / (float)height, 1.0, 1000.0);
		glMatrixMode(GL_MODELVIEW);
		SceneManager::getInstance()->render();
		glAccum(GL_ACCUM, 1.0 / aaSamples);
		cout << ".";
		cout.flush();
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)width / (float)height, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glAccum(GL_RETURN, 1.0);

	//
	fprintf(fp, "P6\n%d %d\n%d\n", width, height, 255);
	for(int y = height - 1; y >= 0; y--) {
		glReadPixels(0, y, width, 1, GL_RGB, GL_UNSIGNED_BYTE, scanline);
		fwrite(scanline, 3, width, fp);
	}
	fclose(fp);

	cout << endl << "Snapshot saved to '" << path << "'." << endl; 
	return true;
}

void RenderEngine::render() const
{
	// Render the entire scene
	SceneManager::getInstance()->render();

	// Buffer swap does implicit glFlush()
	if(doubleBuffer)
		glXSwapBuffers(dpy, win);
	else 
		glFlush();
}

void RenderEngine::render(const Object *obj) const
{
	obj->transformBegin();

	if(obj->displayList)
		glCallList(obj->displayList);
	else
		obj->render();

	obj->transformEnd();
}

SceneFactory::~SceneFactory()
{
	for(map<string, Texture2D*>::iterator it = textures.begin(); it != textures.end(); it++) {
		cout << "[D] Texture2D='" << (it->first) << "'" << endl;
		delete it->second;
	}

	for(map<string, Object*>::iterator it = meshes.begin(); it != meshes.end(); it++) {
		cout << "[D] Mesh=" << "'" << (it->first) << "'" << endl;
		delete it->second;
	}

	for(list<Object*>::iterator it = objects.begin(); it != objects.end(); it++) {
		cout << "[D] Object=" << (*it) << endl;
		delete *it;
	}

	for(map<string, Camera*>::iterator it = cameras.begin(); it != cameras.end(); it++) {
		cout << "[D] Camera=" << "'" << (it->first) << "'" << endl;
		delete it->second;
	}

	for(map<string, Light*>::iterator it = lights.begin(); it != lights.end(); it++) {
		cout << "[D] Light=" << "'" << (it->first) << "'" << endl;
		delete it->second;
	}
}

Texture2D* SceneFactory::loadTexture2D(const string &path)
{
map<string, Texture2D*>::iterator it = textures.find(path);

	if(it == textures.end()) {
		Texture2D *t = new Texture2D(path);
		textures[path] = t;
		cout << "[L] Texture2D='" << path << "'" << endl;
		return t;
	}

	return it->second;
}

Object* SceneFactory::loadMesh(const string &path)
{
map<string, Object*>::iterator it = meshes.find(path);
Object *obj;

	if(it == meshes.end()) {
		obj = new Mesh(path);
		meshes[path] = obj;
		cout << "[L] Mesh='" << path << "', ";
		obj->prerender();
		cout << "dl=" << obj->displayList << endl;
		return obj;
	}

	obj = new Object(it->second->displayList);
	cout << "[C] Object=" << obj << " (from mesh='" << path << "', dl=" << obj->displayList << ")" << endl;
	objects.push_back(obj);
	return obj;
}

Sphere* SceneFactory::createSphere(float radius, int depth, bool reverseNormals)
{
Sphere *p = new Sphere(radius, depth, reverseNormals);

	objects.push_back(p);
	cout << "[C] Object=" << p << " (sphere)" << endl;
	return p;
}

Plane* SceneFactory::createPlane(float width, float height)
{
Plane *p = new Plane(width, height);

	objects.push_back(p);
	cout << "[C] Object=" << p << " (plane)" << endl;
	return p;
}

Cube* SceneFactory::createCube(float edge, bool reverseNormals)
{
Cube *p = new Cube(edge, reverseNormals);

	objects.push_back(p);
	cout << "[C] Object=" << p << " (cube)" << endl;
	return p;
}

SkyBox* SceneFactory::createSkyBox(const string &texPath, float edge)
{
SkyBox *p = new SkyBox(texPath, edge);

	objects.push_back(p);
	cout << "[C] Object=" << p << " (skybox, dl=";
	p->prerender();
	cout << p->displayList << ")" << endl;
	return p;
}

Camera* SceneFactory::createCamera(const string &name)
{
	if(getCamera(name))
		return 0;

	cout << "[C] Camera='" << name << "'" << endl;
	return (cameras[name] = new Camera());
}

Camera* SceneFactory::createCamera(const string &name, const Vector3f &pos, const Vector3f &center, const Vector3f &viewup)
{
	if(getCamera(name))
		return 0;

	cout << "[C] Camera='" << name << "'" << endl;
	return (cameras[name] = new Camera(pos, center, viewup));
}

Light* SceneFactory::createLight(const std::string &name)
{
	if(getLight(name))
		return 0;

	cout << "[C] Light='" << name << "'" << endl;
	return (lights[name] = new Light());
}

int SceneManager::addLight(Light *light)
{ 
int lightId = lights.size();

	if(lightId >= LIGHTS_MAX) 
		return -1;

	glEnable(GL_LIGHT0 + lightId);
	lights.push_back(light);
	return lightId;
}

void SceneManager::render() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(currentCamera)
		currentCamera->activate();

	for(unsigned i = 0; i < lights.size(); i++)
		lights[i]->activate(i);

	for(list<mre::Object*>::const_iterator it = scene.begin(); it != scene.end(); it++) 
		RenderEngine::getInstance()->render(*it);
}

void SceneManager::run()
{
	if(needClear) {
		scene.clear();
		needClear = false; 
	}

	if(!insertList.empty()) {
		for(list<mre::Object*>::iterator it = insertList.begin(); it != insertList.end(); it++) 
			scene.push_back(*it);
		insertList.clear();
	}

	for(list<mre::Object*>::iterator it = scene.begin(); it != scene.end(); it++) {
		(*it)->run();
	}
}

SkyBox::SkyBox(const string &texPath, float edge)
{
	this->edge = edge;
	ft = SceneFactory::getInstance()->loadTexture2D(texPath + "_ft.ppm");
	bk = SceneFactory::getInstance()->loadTexture2D(texPath + "_bk.ppm");
	up = SceneFactory::getInstance()->loadTexture2D(texPath + "_up.ppm");
	dn = SceneFactory::getInstance()->loadTexture2D(texPath + "_dn.ppm");
	lf = SceneFactory::getInstance()->loadTexture2D(texPath + "_lf.ppm");
	rt = SceneFactory::getInstance()->loadTexture2D(texPath + "_rt.ppm");
	material.setAmbient(1.0, 1.0, 1.0, 1.0);
	material.setDiffuse(0.0, 0.0, 0.0, 1.0);
	material.setSpecular(0.0, 0.0, 0.0, 1.0);
	material.setShininess(0.0);
}

void SkyBox::render() const
{
float h = edge * 0.5;

	material.bind();

	ft->bind();
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-h, -h, h);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-h, h, h);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(h, h, h);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(h, -h, h);
	glEnd();
	ft->unbind();

	bk->bind();
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-h, -h, -h);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(h, -h, -h);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(h, h, -h);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-h, h, -h);
	glEnd();
	bk->unbind();

	up->bind();
	glBegin(GL_QUADS);
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-h, h, -h);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(h, h, -h);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(h, h, h);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-h, h, h);
	glEnd();
	up->unbind();

	dn->bind();
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-h, -h, -h);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-h, -h, h);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(h, -h, h);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(h, -h, -h);
	glEnd();
	dn->unbind();

	lf->bind();
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-h, -h, -h);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-h, h, -h);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-h, h, h);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-h, -h, h);
	glEnd();
	lf->unbind();

	rt->bind();
	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(h, -h, -h);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(h, -h, h);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(h, h, h);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(h, h, -h);
	glEnd();
	rt->unbind();

	material.unbind();
}

// Triangle subdivision using vertex numbers
// righthand rule applied to create outward pointing faces
void Sphere::divide_triangle(Vector3f &a, Vector3f &b, Vector3f &c, int depth) const
{
Vector3f v1, v2, v3;
int j;

	if(depth > 0)
	{
		for(j = 0; j < 3; j++)
			v1[j] = a[j] + b[j];
		v1.normalize();

		for(j = 0; j < 3; j++)
			v2[j] = a[j] + c[j];
		v2.normalize();

		for(j = 0; j < 3; j++)
			v3[j] = b[j] + c[j];
		v3.normalize();

		divide_triangle(a, v1, v2, depth - 1);
		divide_triangle(c, v2, v3, depth - 1);
		divide_triangle(b, v3, v1, depth - 1);
		divide_triangle(v1, v3, v2, depth - 1);
	}
	else {
		// Draw triangle at end of recursion 
		if(reverseNormals) {
			glNormal3fv(*b);
			glVertex3f(radius * b[0], radius * b[1], radius * b[2]);
			glNormal3fv(*a);
			glVertex3f(radius * a[0], radius * a[1], radius * a[2]);
		}
		else {
			glNormal3fv(*a);
			glVertex3f(radius * a[0], radius * a[1], radius * a[2]);
			glNormal3fv(*b);
			glVertex3f(radius * b[0], radius * b[1], radius * b[2]);
		}
		glNormal3fv(*c);
		glVertex3f(radius * c[0], radius * c[1], radius * c[2]);
	}
}

void Sphere::render() const
{
Vector3f v1(0.0, 0.0, 1.0);
Vector3f v2(0.0, 0.942809, -0.33333);
Vector3f v3(-0.816497, -0.471405, -0.333333);
Vector3f v4(0.816497, -0.471405, -0.333333);

	material.bind();

	glBegin(GL_TRIANGLES);
	divide_triangle(v1, v2, v3, depth);
	divide_triangle(v4, v3, v2, depth);
	divide_triangle(v1, v4, v2, depth);
	divide_triangle(v1, v3, v4, depth);
	glEnd();

	material.unbind();
}

Texture2D::Texture2D(const string &path)
{
void *data = mre_load_ppm(path.c_str(), &width, &height, &depth);

	//glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//glDisable(GL_TEXTURE_2D);
	free(data);
}

void Texture2D::bind() const 
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
}

void Texture2D::unbind() const 
{
	glDisable(GL_TEXTURE_2D);
}

};

