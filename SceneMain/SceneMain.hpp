#ifndef SCENEMAIN_HPP
#define SCENEMAIN_HPP
#include "Scene.hpp"
#include "SceneMain/World.hpp"
#include "SceneMain/Player.hpp"

class SceneMain : public Scene {
	public:
		//virtual overrides
		SceneMain(Game &parent);
		~SceneMain();
		bool init();
		void update(float deltaTime);
		void draw() const;
		void onKeyPressed(float deltaTime, const sf::Event& event);
		void onMouseButtonPressed(float deltaTime, const sf::Event& event);
		void onMouseMoved(float deltaTime, const sf::Event &event);
		void onClose(); //close scene-wide stuff
			
	private:
		bool loadResources();

		int WORLDSEED;
		Player player;
		sf::Mouse mouse;
		float grassTimer;
		World world;
		float debugCounter;
};

#endif // SCENEMAIN_HPP
