/* mge.cc - Game Engine implementation
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
#include "mge/GameEngine"
#include "mge/Object"
#include <iomanip>
#include <fstream>

using namespace std;

namespace mge {

GameEngine *GameEngine::instance = 0;

GameEngine::GameEngine()
{
	originalGrid = grid = 0;
}

int GameEngine::loadGrid(const string &path)
{
ifstream in(path.c_str());
string magic;
char dot;
int s, i, j, rows, cols;
int version[2];

	if(grid) {
		for(unsigned row = 0; row < grid->getRows(); row++) 
			for(unsigned col = 0; col < grid->getCols(); col++) 
				if(grid->get(row, col))
					delete grid->get(row, col);
		delete grid;
	}
	grid = 0;

	in >> magic;
	if(magic != "MGEGRID") {
		in.close();
		return -1;
	}

	// Not used
	in >> version[0] >> dot >> version[1];

	in >> rows >> cols;
	if(rows <= 0 || cols <= 0) {
		in.close();
		return -2;
	}

	grid = new Grid(rows, cols);
	for(i = 0; i < rows; i++) {
		for(j = 0; j < cols; j++) {
			in >> s;
			grid->set(i, j, this->create((mge::Object::Symbol)s, i, j));
			if(s == mge::Object::SYMBOL_PLAYER) 
				player = grid->get(i, j);
		}
	}

	in.close();
	if(originalGrid)
		delete originalGrid;
	originalGrid = new Grid(*grid);
	return 0;
}

void GameEngine::reloadGrid()
{
	if(originalGrid) {
		if(grid)
			delete grid;
		grid = new Grid(*originalGrid);
		for(unsigned i = 0; i < grid->getRows(); i++) {
			for(unsigned j = 0; j < grid->getCols(); j++) {
				if(grid->get(i, j)) 
					grid->get(i, j)->reset(i, j);
			}
		}
	}
}

GameEngine::~GameEngine()
{
	if(grid)
		delete grid;
	if(originalGrid)
		delete originalGrid;
}

GameEngine::Action GameEngine::movePlayer(Direction dir)
{
unsigned newRow = player->getRow(), newCol = player->getCol(), nbRow, nbCol;

	switch(dir) {
		case DIR_NORTH:
			newRow--;
			break;
		case DIR_SOUTH:
			newRow++;
			break;
		case DIR_WEST:
			newCol--;
			break;
		case DIR_EAST:
			newCol++;
			break;
	}

	if(newRow < 0 || newCol < 0 || newRow >= grid->getRows() || newCol >= grid->getCols())
		return ACTION_NONE;

	if(!grid->get(newRow, newCol)) {
		grid->set(player->getRow(), player->getCol(), 0);
		grid->set(newRow, newCol, player);
		player->setRow(newRow);
		player->setCol(newCol);
		return ACTION_WALK;
	}

	if(grid->get(newRow, newCol)->getSymbol() == mge::Object::SYMBOL_EXIT) {
		return ACTION_EXIT;
	}
	else if(grid->get(newRow, newCol)->moveable()) {
		nbRow = 2 * newRow - player->getRow();
		nbCol = 2 * newCol - player->getCol();
		if(nbRow >= 0 && nbCol >= 0 && nbRow < grid->getRows() && nbCol < grid->getCols() && !grid->get(nbRow, nbCol))
		{
		mge::Object *obj = grid->get(newRow, newCol);

			grid->set(nbRow, nbCol, obj);
			obj->setRow(nbRow);
			obj->setCol(nbCol);

			grid->set(player->getRow(), player->getCol(), 0);
			grid->set(newRow, newCol, player);
			player->setRow(newRow);
			player->setCol(newCol);
			return ACTION_PUSH;
		}
	}

	return ACTION_NONE;
}

};

