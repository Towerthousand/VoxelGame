#include "Game.hpp"
#include<unistd.h>

//short main is bettah
int main() {

	Game myGame;
	if(myGame.init()) {
		myGame.setScene(new SceneMain(myGame));
		myGame.run();
	}
	return 42;
}
