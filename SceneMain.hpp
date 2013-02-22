#ifndef SCENE_CLASS_INTRO_H
#define SCENE_CLASS_INTRO_H
#include "World.hpp"
#include "Scene.hpp"
#include "Camera.hpp"

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
		Camera player;
		sf::Mouse mouse;
		float grassTimer;
		World world;
};

#endif
