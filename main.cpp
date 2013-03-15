#include "Game.hpp"

int main() {
	Game myGame;
	if(myGame.init()) {
		myGame.setScene(new SceneMain(myGame));
		myGame.run();
	}
	return 42;
}
