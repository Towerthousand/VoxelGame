#include "Game.hpp"

//extern'd in tools.hpp
int SCRWIDTH = 500; //1366
int SCRHEIGHT = 500; //768
bool WINDOWFOCUS = false;

//short main is bettah
int main() {
	Game myGame;
	if(myGame.init()) {
		myGame.setScene(new SceneMain(myGame));
		myGame.run();
	}
	return 42;
}
