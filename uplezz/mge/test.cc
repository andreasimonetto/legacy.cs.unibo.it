#include "mge/GameEngine"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace mge;

class Player : public Object {
	public:
		Player(unsigned row, unsigned col) : Object(row, col) { }
		int getSymbol() const { return Object::SYMBOL_PLAYER; }
		virtual void render() const { }
};

class Water : public Object {
	public:
		Water(unsigned row, unsigned col) : Object(row, col) { }
		int getSymbol() const { return Object::SYMBOL_WATER; }
		virtual void render() const { }
};

class Fountain : public Object {
	public:
		Fountain(unsigned row, unsigned col) : Object(row, col) { }
		int getSymbol() const { return Object::SYMBOL_FOUNTAIN; }
		virtual void render() const { }
		virtual bool moveable() const { return true; }
};

class Tree : public Object {
	public:
		Tree(unsigned row, unsigned col) : Object(row, col) { }
		int getSymbol() const { return Object::SYMBOL_TREE; }
		virtual void render() const { }
};

class Exit : public Object {
	public:
		Exit(unsigned row, unsigned col) : Object(row, col) { }
		int getSymbol() const { return Object::SYMBOL_EXIT; }
		virtual void render() const { }
};

class TestGameEngine : public GameEngine {
	private:
		TestGameEngine() : GameEngine() { }
	public:
		static void init() {
			if(!instance)
				instance = new TestGameEngine();
		}

		mge::Object* create(mge::Object::Symbol s, unsigned row, unsigned col) {
			switch(s) {
				case mge::Object::SYMBOL_PLAYER:
					return new Player(row, col);
					break;
				case mge::Object::SYMBOL_WATER:
					return new Water(row, col);
					break;
				case mge::Object::SYMBOL_FOUNTAIN:
					return new Fountain(row, col);
					break;
				case mge::Object::SYMBOL_TREE:
					return new Tree(row, col);
					break;
				case mge::Object::SYMBOL_EXIT:
					return new Exit(row, col);
					break;
				default:
					return 0;
					break;
			}
		}

		void render() const
		{
		unsigned i, j;

			if(!grid) {
				cout << "<grid not loaded>" << endl;
				return;
			}

			for(i = 0; i < grid->getRows(); i++) {
				for(j = 0; j < grid->getCols(); j++) {
					if(grid->get(i, j))
						cout << setw(2) << grid->get(i, j)->getSymbol() << ' ';
					else
						cout << "-- ";
				}
				cout << endl;
			}
		}
};

int main(int argc, char *argv[])
{
bool quit = false;
char command;
const char *grid = "../data/grids/test.grid";

	if(argc > 1)
		grid = argv[1];

	TestGameEngine::init();

	GameEngine::getInstance()->loadGrid(grid);
	while(!cin.eof() && !quit)
	{
	GameEngine::Action action = GameEngine::ACTION_NONE;

		GameEngine::getInstance()->render();
		cout << "Command: ";
		cin >> command;
		switch(command) {
			case 'w':
				action = GameEngine::getInstance()->movePlayer(GameEngine::DIR_NORTH);
				break;
			case 'z':
				action = GameEngine::getInstance()->movePlayer(GameEngine::DIR_SOUTH);
				break;
			case 'a':
				action = GameEngine::getInstance()->movePlayer(GameEngine::DIR_WEST);
				break;
			case 's':
				action = GameEngine::getInstance()->movePlayer(GameEngine::DIR_EAST);
				break;
			case 'q':
				quit = true;
				break;
		}

		if(action == GameEngine::ACTION_EXIT)
			quit = true;
	}

	return 0;
}

