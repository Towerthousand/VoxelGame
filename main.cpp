#include "Game.hpp"

//extern'd in tools.hpp
int SCRWIDTH = 1920; //1366
int SCRHEIGHT = 1080; //768

//short main is bettah
int main() {
	Game myGame;
	if(myGame.init()) {
		myGame.setScene(new SceneMain(myGame));
		myGame.run();
	}
	return 42;
}
