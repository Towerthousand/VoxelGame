#include "Game.hpp"

//extern'd in tools.hpp
int SCRWIDTH = 1366; //1366
int SCRHEIGHT = 768; //768
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
