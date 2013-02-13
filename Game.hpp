#ifndef GAME_CLASS_H
#define GAME_CLASS_H
#include "tools.hpp"
#include "MediaManager.hpp"
#include "Scene.hpp"
#include "SceneMain.hpp"

class Game {
	public:
		Game(); //creates window
		~Game();
		bool init(); // inits game-wide stuff here (init and resource loading)
		void run(); // won't run if Game::init() isn't called first
					// contains main loop, calls update() and draw()
		void update(float deltaTime); // changes scene if necessary
									  // updates fps
									  // checks for window events
									  // updates input with
									  // Game::onMouseButtonPressed(); and
									  // Game::onKeyPressed();
									  // calls currentScene.update(deltaTime)
		void draw(); // calls currentScene.draw()
		void onClose(); // calls currentScene.onClose(), and then Game::close()
						// currentScene.onClose() will save scene specific stuff
						// and then call for the whole game to save game-wide
						// stuff and shut down		
		void close(); // closes app completely, saves game-wide stuff first
		void setScene(Scene * scene); // sets nextScene to scene, which will
								      // move into currentScene on next update()
								      // so that there is no update() of one
								      // scene followed by a draw() method
								      // of a different scene.

		sf::RenderWindow &getWindow() { return window; }
		
		bool isRunning;
		MediaManager resources;
		
	private:
		bool loadResources (); // loads game-wide resources. only called
									 // by init() once
		void onKeyPressed(float deltaTime);
									 	// currentScene.onKeyPressed
										// (event, deltaTime)
		void onMouseButtonPressed(float deltaTime);
									 	// currentScene.onMouseButtonPressed
										// (deltaTime)

		sf::RenderWindow window;
		Scene* currentScene;
		Scene* nextScene;
		float fpsTime;
		int fpsCount;	
};

#endif
