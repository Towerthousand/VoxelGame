#ifndef SCENE_CLASS_INTRO_H
#define SCENE_CLASS_INTRO_H
#include "World.hpp"
#include "Scene.hpp"
#include "Player.hpp"

class SceneMain : public Scene {
	public:
		//virtual overrides
		SceneMain(Game &parent);
		~SceneMain();
		bool init();
		void update(const float& deltaTime);
		void draw() const;
		void onKeyPressed(const float& deltaTime, const sf::Event& event);
		void onMouseButtonPressed(const float& deltaTime, const sf::Event& event);
		void onMouseMoved(const float& deltaTime, const sf::Event &event);
		void onClose(); //close scene-wide stuff
			
	private:
		bool loadResources();

		int WORLDSEED;
		Player player;
		sf::Mouse mouse;
		float grassTimer;
		World world;
};

#endif
