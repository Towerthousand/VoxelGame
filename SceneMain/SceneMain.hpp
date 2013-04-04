#ifndef SCENEMAIN_HPP
#define SCENEMAIN_HPP
#include "Scene.hpp"
#include "SceneMain/World.hpp"
#include "SceneMain/Player.hpp"

class Arrow;
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
		void onMouseMoved(float deltaTime, int dx, int dy);
		void onClose(); //close scene-wide stuff

	private:
		bool loadResources();

		int WORLDSEED;
		Player player;
		sf::Mouse mouse;
		World world;
		std::vector<GameObject*> objects;
		float debugCounter;
		int fpsCount;
};

#endif // SCENEMAIN_HPP
