/* uplezz.cc - Implementation of the game
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
#include "Exit"
#include "Fountain"
#include "Tree"
#include "Player"
#include "UplezzEngine"
#include "UplezzTimer"
#include "SplashScreen"
#include "Water"
#include "mge/Object"
#include "mge/GameEngine"
#include "mre/alpha.h"
#include "mre/image.h"
#include "mre/Camera"
#include "mre/Light"
#include "mre/Object"
#include "mre/RenderEngine"
#include "mre/SceneFactory"
#include "mre/SceneManager"
#include "mre/SkyBox"
#include "sys/Clock"
#include "sys/ClockViewer"
#include "sys/System"
#include "sys/Timer"

#include <dirent.h>
#include <math.h>
#include <fnmatch.h>
#include <stdlib.h>
#include <list>
#include <string>

static const char *textures[] = {
	"data/skybox/islands_bk.ppm",
	"data/skybox/islands_dn.ppm",
	"data/skybox/islands_ft.ppm",
	"data/skybox/islands_lf.ppm",
	"data/skybox/islands_rt.ppm",
	"data/skybox/islands_up.ppm",
	"data/textures/terra.ppm",
	"data/textures/water2.ppm",
	0
};

static const char *meshes[] = {
	"data/elliott/idle.obj",
	"data/elliott/eye_tick1.obj",
	"data/elliott/click1.obj",
	"data/elliott/click2.obj",
	"data/elliott/click3.obj",
	"data/elliott/die1.obj",
	"data/elliott/die2.obj",
	"data/elliott/die3.obj",
	"data/elliott/die4.obj",
	"data/elliott/die5.obj",
	"data/elliott/die6.obj",
	"data/elliott/die7.obj",
	"data/elliott/die8.obj",
	"data/elliott/die9.obj",
	"data/elliott/die10.obj",
	"data/elliott/die11.obj",
	"data/elliott/die12.obj",
	"data/elliott/die13.obj",
	"data/elliott/push1.obj",
	"data/elliott/push2.obj",
	"data/elliott/push3.obj",
	"data/elliott/push4.obj",
	"data/elliott/push5.obj",
	"data/elliott/push6.obj",
	"data/elliott/push7.obj",
	"data/elliott/push8.obj",
	"data/elliott/push_end1.obj",
	"data/elliott/push_end2.obj",
	"data/elliott/push_start1.obj",
	"data/elliott/push_start2.obj",
	"data/elliott/walk1.obj",
	"data/elliott/walk2.obj",
	"data/elliott/walk3.obj",
	"data/elliott/walk4.obj",
	"data/elliott/walk5.obj",
	"data/elliott/walk6.obj",
	"data/elliott/walk7.obj",
	"data/elliott/walk8.obj",
	"data/elliott/walk_end1.obj",
	"data/elliott/walk_end2.obj",
	"data/elliott/walk_start1.obj",
	"data/elliott/walk_start2.obj",
	"data/objects/exit.obj",
	"data/objects/fountain.obj",
	0
};

static const unsigned ObjectsColor[] = {
	0x7F000000,
	0x7F00FFFF,
	0x7FFF0000,
	0x7FFFFF00,
	0x7F00FF00,
	0x7F0000FF
};

using namespace std;
using namespace mre;
using namespace mge;
using namespace sys;

static int grids_dir_filter(const struct dirent *entry)
{
	return !fnmatch("level??.grid", entry->d_name, 0);
}

Player::Player(const string &path, unsigned row, unsigned col) : 
	mge::Object(row, col),
	walkAnim(8, path + "/walk", 12),
	walkStartAnim(2, path + "/walk_start", 12),
	walkEndAnim(2, path + "/walk_end", 12),
	pushAnim(8, path + "/push", 12),
	pushStartAnim(2, path + "/push_start", 12),
	pushEndAnim(2, path + "/push_end", 12),
	clickAnim(3, path + "/click", 8),
	dieAnim(13, path + "/die", 12),
	eyesTickAnim(1, path + "/eye_tick", 12),
	rotTimer(10000),
	cameraTimer(1000)
{
	action = mge::GameEngine::ACTION_NONE;
	fountain = 0;

	curFrame = idleFrame = SceneFactory::getInstance()->loadMesh(path + "/idle.obj");
	cameraIsRotating = cameraIsResetting = false;
	cameraDeltaD = cameraDeltaTheta = cameraDeltaPhi = 0.0;
	status = STATUS_IDLE;
	rotCount = 0;
	cameraPhi = 0.0;
	cameraTheta = M_PI * 0.4;
	cameraD = 16.0;
	rotate(90.0, mre::Object::AXIS_Y);
	move(CELL_SIZE * getRow() + 0.5 * CELL_SIZE, 0.0, CELL_SIZE * getCol() + 0.5 * CELL_SIZE);

	walkStartAnim.setFrameCallback(this, &Player::actionStartCallback);
	walkAnim.setFrameCallback(this, &Player::actionCallback);
	pushAnim.setFrameCallback(this, &Player::actionCallback);
	//SceneManager::getInstance()->addObject(this);

	camera = SceneFactory::getInstance()->getCamera("PlayerCamera");
	SceneManager::getInstance()->setCurrentCamera(camera);
	refreshCamera();

	RenderEngine::getInstance()->postRedisplay();

	exitAction = stopAction = false;
}

Player::~Player()
{

}

bool Player::actionStartCallback(unsigned cycle, unsigned frame)
{
mge::GameEngine::Direction dir = getDirection();
mge::Grid *grid = mge::GameEngine::getInstance()->getGrid();

	if(frame > 0)
		return true;

	action = mge::GameEngine::getInstance()->movePlayer(dir);
	if(action == mge::GameEngine::ACTION_NONE) {
		status = STATUS_IDLE;
		return false;
	}
	else if(action == mge::GameEngine::ACTION_PUSH) {
		if(dir == mge::GameEngine::DIR_NORTH)
			fountain = (Fountain*)grid->get(row - 1, col);
		else if(dir == mge::GameEngine::DIR_SOUTH)
			fountain = (Fountain*)grid->get(row + 1, col);
		else if(dir == mge::GameEngine::DIR_WEST)
			fountain = (Fountain*)grid->get(row, col - 1);
		else
			fountain = (Fountain*)grid->get(row, col + 1);

		status = STATUS_PUSH_TRY;
		return false;
	}

	return true;
}

bool Player::actionCallback(unsigned cycle, unsigned frame)
{
mge::GameEngine::Direction dir = getDirection();

	if(cycle > 0 && frame == 0)
	{
	bool isPushing = (action == mge::GameEngine::ACTION_PUSH);

		if(!actionStartCallback(0, 0)) {
			if(action == mge::GameEngine::ACTION_NONE) {
				status = isPushing ? STATUS_PUSH_STOPPING : STATUS_WALK_STOPPING;
				return false;
			}
			else if(action == mge::GameEngine::ACTION_PUSH && isPushing) {
				status = STATUS_PUSHING;
			}
			else
				return false;
		}
	}

	if(frame == 0 && action == mge::GameEngine::ACTION_EXIT) {
		status = STATUS_EXIT_START;
		walkAnim.setLoop(false);
	}

	switch(dir) {
		case mge::GameEngine::DIR_NORTH:
			move(-CELL_SIZE / 8.0, 0.0, 0.0);
			if(fountain)
				fountain->move(-CELL_SIZE / 8.0, 0.0, 0.0);
			break;
		case mge::GameEngine::DIR_SOUTH:
			move(CELL_SIZE / 8.0, 0.0, 0.0);
			if(fountain)
				fountain->move(CELL_SIZE / 8.0, 0.0, 0.0);
			break;
		case mge::GameEngine::DIR_WEST:
			move(0.0, 0.0, -CELL_SIZE / 8.0);
			if(fountain)
				fountain->move(0.0, 0.0, -CELL_SIZE / 8.0);
			break;
		case mge::GameEngine::DIR_EAST:
			move(0.0, 0.0, CELL_SIZE / 8.0);
			if(fountain)
				fountain->move(0.0, 0.0, CELL_SIZE / 8.0);
			break;
	}

	refreshCamera();
	return true;
}

void Player::cameraRotate(int x, int y)
{
	if(cameraIsRotating) {
		cameraPhi += ((float)x - cameraX) * M_PI / 360.0;
		while(cameraPhi < 0)
			cameraPhi += 2 * M_PI;
		while(cameraPhi >= 2 * M_PI)
			cameraPhi -= 2 * M_PI;

		cameraTheta -= ((float)y - cameraY) * M_PI / 360.0;
		if(cameraTheta < 0.0078125)
			cameraTheta = 0.0078125;
		else if(cameraTheta > M_PI * 0.75)
			cameraTheta = M_PI * 0.75;

		cameraX = x;
		cameraY = y;
		refreshCamera();
		RenderEngine::getInstance()->postRedisplay();
	}
}

void Player::run()
{
	curFrame->run();
	if(status == STATUS_IDLE) {
		if(eyesTickTimer.hasExpired()) 
			tickEyes();
	}
	else if(status == STATUS_WALK_START && walkStartAnim.isEnd()) {
		curFrame = &walkAnim;
		walkAnim.start(true);
		status = STATUS_WALKING;
	}
	else if(status == STATUS_WALKING && stopAction) {
		stopAction = false;
		walkAnim.setLoop(false);
		status = STATUS_WALK_STOPPING;
	}
	else if(status == STATUS_WALK_STOPPING && walkAnim.isEnd()) {
		curFrame = &walkEndAnim;
		walkEndAnim.start();
		status = STATUS_WALK_END;
	}
	else if(status == STATUS_WALK_END && walkEndAnim.isEnd()) {
		status = STATUS_IDLE;
		curFrame = idleFrame;
		refreshCamera();
		RenderEngine::getInstance()->postRedisplay();
	}
	else if(status == STATUS_EYES_TICK && eyesTickAnim.isEnd()) {
		status = STATUS_IDLE;
		curFrame = idleFrame;
		refreshCamera();
		RenderEngine::getInstance()->postRedisplay();
		eyesTickTimer.restart(1000000 * (random() % 20));
	}
	else if(status == STATUS_ROTATING && rotTimer.hasExpired()) {
		rotate(rotDir * 9, mre::Object::AXIS_Y);
		if(++rotCount == 10) {
			status = STATUS_IDLE;
			curFrame = idleFrame;
			RenderEngine::getInstance()->postRedisplay();
			rotCount = 0;
		}
		else {
			rotTimer.restart();
			RenderEngine::getInstance()->postRedisplay();
		}
	}
	else if(status == STATUS_CLICKING && clickAnim.isEnd()) {
		status = STATUS_IDLE;
		curFrame = idleFrame;
		refreshCamera();
		RenderEngine::getInstance()->postRedisplay();
	}
	else if(status == STATUS_PUSH_TRY) {
		curFrame = &pushStartAnim;
		pushStartAnim.start();
		status = STATUS_PUSH_START;
	}
	else if(status == STATUS_PUSH_START && pushStartAnim.isEnd()) {
		curFrame = &pushAnim;
		pushAnim.start(true);
		status = STATUS_PUSHING;
	}
	else if(status == STATUS_PUSHING && stopAction) {
		stopAction = false;
		pushAnim.setLoop(false);
		status = STATUS_PUSH_STOPPING;
	}
	else if(status == STATUS_PUSH_STOPPING && pushAnim.isEnd()) {
		fountain = 0;
		curFrame = &pushEndAnim;
		pushEndAnim.start();
		status = STATUS_PUSH_END;
	}
	else if(status == STATUS_PUSH_END && pushEndAnim.isEnd()) {
		status = STATUS_IDLE;
		curFrame = idleFrame;
		refreshCamera();
		RenderEngine::getInstance()->postRedisplay();
	}
	else if(status == STATUS_DIE && dieAnim.isEnd()) {
		status = STATUS_IDLE;
		curFrame = idleFrame;
		mge::GameEngine::getInstance()->reloadGrid();

	}
	else if(status == STATUS_EXIT_START && walkAnim.isEnd()) {
		curFrame = &walkEndAnim;
		walkEndAnim.start();
		status = STATUS_EXIT;
	}
	else if(status == STATUS_EXIT && walkEndAnim.isEnd()) {
		status = STATUS_IDLE;
		curFrame = idleFrame;
		//refreshCamera();
		//RenderEngine::getInstance()->postRedisplay();
		exitAction = true;
	}

	if(cameraIsResetting && cameraTimer.hasExpired())
	{
	float delta;

		cameraIsResetting = false;
		if(cameraDeltaD != 0.0)  {
			delta = 0.03125 * (cameraDeltaD < 0.0 ? 1.0 : -1.0);
			if(fabs(cameraDeltaD) < fabs(delta)) {
				cameraDeltaD = 0.0;
				cameraD = 16.0;
			}
			else {
				cameraDeltaD += delta;
				cameraD -= delta;
			}
			cameraIsResetting = true;
		}

		if(cameraDeltaPhi != 0.0) {
			delta = M_PI / 360.0 * (cameraDeltaPhi < 0.0 ? 1.0 : -1.0);
			if(fabs(cameraDeltaPhi) < fabs(delta)) {
				cameraDeltaPhi = 0.0;
				cameraPhi = 0.0;
			}
			else {
				cameraDeltaPhi += delta;
				cameraPhi -= delta;
			}
			cameraIsResetting = true;
		}

		if(cameraDeltaTheta != 0.0) {
			delta = M_PI / 360.0 * (cameraDeltaTheta < 0.0 ? 1.0 : -1.0);
			if(fabs(cameraDeltaTheta) < fabs(delta)) {
				cameraDeltaTheta = 0.0;
				cameraTheta = 0.4 * M_PI;
			}
			else {
				cameraDeltaTheta += delta;
				cameraTheta -= delta;
			}
			cameraIsResetting = true;
		}

		if(cameraIsResetting) {
			refreshCamera();
			RenderEngine::getInstance()->postRedisplay();
			cameraTimer.restart();
		}
	}
}

UplezzEngine::UplezzEngine(const string &playerP) : 
	GameEngine(),
	playerPath(playerP)
{
struct dirent **namelist;
int n = scandir("data/grids", &namelist, grids_dir_filter, alphasort);

	if(n > 0) {
		//
		while(n--) {
			levels.push_front(string("data/grids/") + namelist[n]->d_name);
			free(namelist[n]);
		}

		nextLevel = levels.begin();
		free(namelist);
	}

	skybox = SceneFactory::getInstance()->createSkyBox("data/skybox/islands", 1024.0);
	displayHelpOnline = false;

	Texture2D *terrain = SceneFactory::getInstance()->loadTexture2D("data/textures/terra.ppm");

	tile1 = SceneFactory::getInstance()->createPlane(CELL_SIZE, CELL_SIZE);
	tile1->material.setAmbient(1.0, 1.0, 1.0, 1.0);
	tile1->material.setDiffuse(1.0, 1.0, 1.0);
	tile1->material.setSpecular(0.1, 0.1, 0.1);
	tile1->material.setShininess(2);
	tile1->material.attachTexture(terrain);

	loadGrid(*nextLevel);
	nextLevel++;
	timer.reset();
}

mge::Object* UplezzEngine::create(mge::Object::Symbol s, unsigned row, unsigned col)
{
Player *player;
Water *water;
Fountain *fountain;
Tree *tree;
Exit *exit;

	switch(s) {
		case mge::Object::SYMBOL_PLAYER:
			player = new Player(playerPath, row, col);
			gridObjects.push_back(player);
			return player;
			break;
		case mge::Object::SYMBOL_WATER:
			water = new Water(row, col);
			gridObjects.push_back(water);
			return water;
			break;
		case mge::Object::SYMBOL_FOUNTAIN:
			fountain = new Fountain(row, col);
			gridObjects.push_back(fountain);
			return fountain;
			break;
		case mge::Object::SYMBOL_TREE:
			tree = new Tree(row, col);
			gridObjects.push_back(tree);
			return tree;
			break;
		case mge::Object::SYMBOL_EXIT:
			exit = new Exit(row, col);
			gridObjects.push_back(exit);
			return exit;
			break;
		default:
			return 0;
			break;
	}
}

void UplezzEngine::render() const
{
unsigned i, j;
int gridPosY = RenderEngine::getInstance()->getWindowHeight() - 10;

	if(!grid) 
		return;

	struct {
		unsigned width;
		unsigned height;
		unsigned bytes_per_pixel;
		unsigned char pixel_data[10 * 10 * 4];
	} cellImage;

	cellImage.width = 10;
	cellImage.height = 10;
	cellImage.bytes_per_pixel = 4;

	for(i = 0; i < grid->getRows(); i++) {
		for(j = 0; j < grid->getCols(); j++)
		{
		mge::Object *obj = grid->get(i, j);

			if(!obj || obj->getSymbol() != mge::Object::SYMBOL_WATER) {
				tile1->setPosition((0.5 + i) * CELL_SIZE, 0.0, (0.5 + j) * CELL_SIZE);
				mre::RenderEngine::getInstance()->render(tile1);
			}
		}
	}

	for(list<mre::Object*>::const_iterator it = gridObjects.begin(); it != gridObjects.end(); it++)
		mre::RenderEngine::getInstance()->render(*it);

	for(i = 0; i < grid->getRows(); i++) {
		for(j = 0; j < grid->getCols(); j++)
		{
		mge::Object *obj = grid->get(i, j);
		unsigned x, y, color = ObjectsColor[obj ? obj->getSymbol() : 0];

			for(x = 0; x < 10; x++)
				for(y = 0; y < 10; y++)
					image_setpixel((image_t*)&cellImage, x, y, color);

			RenderEngine::getInstance()->render((image_t*)&cellImage, GRID_X + 10 * j, gridPosY - 10 * (i + 1));
		}
	}

	if(displayHelpOnline)
		RenderEngine::getInstance()->render(helpOnlineImg, (RenderEngine::getInstance()->getWindowWidth() - helpOnlineImg->width) / 2, 30);
}

void UplezzEngine::run()
{
unsigned i, j;

	if(!grid) 
		return;

	for(i = 0; i < grid->getRows(); i++) 
		for(j = 0; j < grid->getCols(); j++) 
			if(grid->get(i, j))
				grid->get(i, j)->run();

	if(getPlayer()->hasExit()) {
		if(nextLevel != levels.end()) {
			loadGrid(*nextLevel);
			nextLevel++;
		}
		else {
			RenderEngine::getInstance()->postQuit();
		}
	}
}

int UplezzEngine::loadGrid(const string &path)
{
	SceneManager::getInstance()->clearScene();
	gridObjects.clear();

	int retval = mge::GameEngine::loadGrid(path);
	if(retval != 0) 
		return retval;

	skybox->setPosition(0.5 * grid->getRows() * CELL_SIZE, 0.0, 0.5 * grid->getCols() * CELL_SIZE);
	SceneManager::getInstance()->addObject(skybox);
	SceneManager::getInstance()->addObject(this);

	//timer.reset();
	SceneManager::getInstance()->addObject(&timer);

	cout << "[L] Level: '" << path << "'" << endl;
	return retval;
}

void UplezzTimer::render() const
{
int x = (RenderEngine::getInstance()->getWindowWidth() - ALPHA_WIDTH) / 2;
ClockViewer curTimeViewer(Clock::now() - start);
int min = curTimeViewer.getMinutes();
int sec = curTimeViewer.getSeconds();

	RenderEngine::getInstance()->render(alpha_images[ALPHA0 + min / 10], x - 2 * (ALPHA_WIDTH + 2), TIMER_Y);
	RenderEngine::getInstance()->render(alpha_images[ALPHA0 + min % 10], x - (ALPHA_WIDTH + 2), TIMER_Y);
	RenderEngine::getInstance()->render(alpha_images[ALPHA_COLON], x, TIMER_Y);
	RenderEngine::getInstance()->render(alpha_images[ALPHA0 + sec / 10], x + (ALPHA_WIDTH + 2), TIMER_Y);
	RenderEngine::getInstance()->render(alpha_images[ALPHA0 + sec % 10], x + 2 * (ALPHA_WIDTH + 2), TIMER_Y);
}

void SplashScreen::render() const
{
	RenderEngine::getInstance()->render(splash1Img, (RenderEngine::getInstance()->getWindowWidth() - splash1Img->width) / 2, 50);
	RenderEngine::getInstance()->render(splash2Img, (RenderEngine::getInstance()->getWindowWidth() - splash2Img->width) / 2, RenderEngine::getInstance()->getWindowHeight() - 50);
}

void SplashScreen::run()
{
	if(rotationTimer.hasExpired()) {
		me->rotate(5.0, mre::Object::AXIS_Y);
		RenderEngine::getInstance()->render();
		rotationTimer.restart();

		if(textures[texId]) 
			SceneFactory::getInstance()->loadTexture2D(textures[texId++]);
		else if(meshes[meshId]) 
			SceneFactory::getInstance()->loadMesh(meshes[meshId++]);
		else {
			SceneManager::getInstance()->clearScene();
			RenderEngine::getInstance()->setKeyPressHandler(keyPressHandle);
			RenderEngine::getInstance()->setKeyReleaseHandler(keyReleaseHandle);
			RenderEngine::getInstance()->setMouseButtonPressHandler(mouseButtonPressHandle);
			RenderEngine::getInstance()->setMouseButtonReleaseHandler(mouseButtonReleaseHandle);
			RenderEngine::getInstance()->setMouseMotionHandler(mouseMotionHandle);
			UplezzEngine::init("data/elliott");
		}
	}
}

