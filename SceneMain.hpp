#ifndef SCENE_CLASS_INTRO_H
#define SCENE_CLASS_INTRO_H
#include "Game.hpp"
#include "World.hpp"

class SceneMain : public Scene {
	public:
		//virtual overrides
		SceneMain(Game &parent);
		~SceneMain();
		bool init();
		void update(float deltaTime);
		void draw();
		void onKeyPressed(float deltaTime);
		void onMouseButtonPressed(float deltaTime);
		void onClose(); //close scene-wide stuff
			
	private:
		bool loadResources();

		int WORLDSEED;
		Camera Player;
		sf::Mouse mouse;
		float grassTimer;
		World world;
};

#endif
