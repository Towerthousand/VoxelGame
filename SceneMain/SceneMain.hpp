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
		void onKeyPressed(float deltaTime, const sf::Keyboard::Key &key);
		void onKeyDown(float deltaTime, const sf::Keyboard::Key &key);
		void onKeyReleased(float deltaTime, const sf::Keyboard::Key &key);
		void onMouseButtonPressed(float deltaTime, const sf::Mouse::Button &button);
		void onMouseButtonDown(float deltaTime, const sf::Mouse::Button &button);
		void onMouseButtonReleased(float deltaTime, const sf::Mouse::Button &button);
		void onMouseMoved(float deltaTime);
		void onClose(); //close scene-wide stuff

	private:
		bool loadResources();

		int WORLDSEED;
		Player player;
		sf::Mouse mouse;
		World world;
		float debugCounter;
		int fpsCount;
};

#endif // SCENEMAIN_HPP
