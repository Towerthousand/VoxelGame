#ifndef GAME_HPP
#define GAME_HPP
#include "graphics/TextureManager.hpp"
#include "graphics/FontManager.hpp"
#include "audio/AudioManager.hpp"
#include "SceneMain/SceneMain.hpp"

class Game {
	public:
		Game(); //creates window
		~Game();
		bool init(); // inits game-wide stuff here (init and resource loading)
		void run(); // won't run if Game::init() isn't called first
					// contains main loop, calls update() and draw()
		void setScene(Scene * scene); // sets nextScene to scene, which will
									  // move into currentScene on next update()
									  // so that there is no update() of one
									  // scene followed by a draw() method
                                      // of a different scene.
        void close(); // closes app completely, closing the current scene (if there is)
                      // first, saves game-wide stuff first.

		sf::RenderWindow &getWindow() { return window; }
		TextureManager &textures() { return texManager; }
		FontManager &font() { return fontManager; }
		AudioManager &audio() { return audioManager; }
		
		bool isRunning;
	private:
		void update(float deltaTime); // changes scene if necessary
									  // updates fps
									  // checks for window events
									  // updates input with
									  // Game::onMouseButtonPressed(); and
									  // Game::onKeyPressed();
									  // calls currentScene.update(deltaTime)
        void draw(); // calls currentScene.draw()
		bool loadResources (); // loads game-wide resources. only called
									 // by init() once
		void onKeyPressed(float deltaTime,const sf::Event& event);
									 	// currentScene.onKeyPressed
										// (deltaTime, event)
		void onMouseButtonPressed(float deltaTime,const sf::Event& event);
									 	// currentScene.onMouseButtonPressed
										// (deltaTime, event)
		void onMouseMoved(float deltaTime,const sf::Event& event);

		//context
		sf::RenderWindow window;
		Scene* currentScene;
		Scene* nextScene;

		//managers
		TextureManager texManager;
		FontManager fontManager;
		AudioManager audioManager;
};

#endif //GAME_HPP
