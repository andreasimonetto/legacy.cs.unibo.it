/* main.cc - Main function and I/O handlers for Uplezz
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
#include "uplezz.h"
#include "UplezzEngine"
#include "SplashScreen"
#include "mre/RenderEngine"
#include "mre/SceneManager"
#include "mre/SceneFactory"
#include "sys/System"

using namespace mge;
using namespace mre;
using namespace sys;

void mouseButtonPressHandle(int button, int state, int x, int y)
{
	switch(button) {
		case 1:
			UplezzEngine::getInstance()->getPlayer()->cameraRotateBegin(x, y);
			break;
	}
}

void mouseButtonReleaseHandle(int button, int state, int x, int y)
{
	switch(button) {
		case 1:
			UplezzEngine::getInstance()->getPlayer()->cameraRotateEnd();
			break;
		case 3:
			UplezzEngine::getInstance()->getPlayer()->cameraReset();
			break;
		case 4:
			UplezzEngine::getInstance()->getPlayer()->cameraDistanceDec();
			break;
		case 5:
			UplezzEngine::getInstance()->getPlayer()->cameraDistanceInc();
			break;
	}
}

void mouseMotionHandle(int state, int x, int y)
{
	UplezzEngine::getInstance()->getPlayer()->cameraRotate(x, y);
}

void keyPressHandle(int key, int state, int x, int y)
{
	switch(key) {
		case XK_Left:
			UplezzEngine::getInstance()->getPlayer()->rotateLeft();
			break;
		case XK_Right:
			UplezzEngine::getInstance()->getPlayer()->rotateRight();
			break;
		case XK_Up:
			UplezzEngine::getInstance()->getPlayer()->startWalk();
			break;
	}
}

void keyReleaseHandle(int key, int state, int x, int y)
{
	switch(key) {
		case XK_F1:
			UplezzEngine::getInstance()->toggleHelpOnline();
			break;
		case XK_F2:
			UplezzEngine::getInstance()->suspendTimer();
			RenderEngine::getInstance()->snapshot();
			UplezzEngine::getInstance()->restartTimer();
			break;
		case XK_r:
		case XK_R:
			UplezzEngine::getInstance()->getPlayer()->die();
			break;
		case XK_Up:
			UplezzEngine::getInstance()->getPlayer()->stopWalk();
			break;
		case XK_Escape:
			RenderEngine::getInstance()->postQuit();
			break;
		case XK_Return:
			UplezzEngine::getInstance()->getPlayer()->cameraReset();
			break;
	}
}

int main(int argc, char *argv[])
{
Light *lite;

	//
	RenderEngine::getInstance()->initMainWindow(argc, argv, WINDOW_TITLE, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//
	SceneFactory::getInstance()->createCamera("PlayerCamera");

	//
	lite = SceneFactory::getInstance()->createLight("DefaultLight");
	lite->setPosition(-1.0, 0.70710678, -0.70710678);
	lite->setAmbient(1.0, 1.0, 1.0);
	lite->setDiffuse(0.75, 0.75, 0.75);
	lite->setSpecular(1.0, 1.0, 1.0);
	SceneManager::getInstance()->addLight(lite);

	//
	SplashScreen splashScreen;
	SceneManager::getInstance()->addObject(&splashScreen);

	//
	RenderEngine::getInstance()->mainLoop();

	//
	System::getInstance()->exit(0);
	return 0;
}

