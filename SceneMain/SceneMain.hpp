#ifndef SCENEMAIN_HPP
#define SCENEMAIN_HPP
#include "../Scene.hpp"
#include "World.hpp"
#include "Player.hpp"

class Skeleton;
class SceneMain : public Scene {
	public:
		//virtual overrides
		SceneMain(Game &parent);
		~SceneMain();
		bool init();
		void update(float deltaTime);
		void draw() const;
		void onKeyPressed(float deltaTime, sf::Keyboard::Key key);
		void onKeyDown(float deltaTime, sf::Keyboard::Key key);
		void onKeyReleased(float deltaTime, sf::Keyboard::Key key);
		void onMouseButtonPressed(float deltaTime, sf::Mouse::Button button);
		void onMouseButtonDown(float deltaTime, sf::Mouse::Button button);
		void onMouseButtonReleased(float deltaTime, sf::Mouse::Button button);
		void onMouseMoved(float deltaTime, int dx, int dy);
		void onClose(); //close scene-wide stuff

		void addObject(GameObject* object);
		World& getWorld();
		int WORLDSEED;
		Player* player; //for easy access, so that you don't have to search the object vector each time

	private:
		bool loadResources();

		World world;
		std::list<GameObject*> objects;
		float debugCounter;
		int fpsCount;
};

#endif // SCENEMAIN_HPP
