#ifndef SCENE_HPP
#define SCENE_HPP
#include "tools.hpp"

class Game;
class Scene {
	public:
		Scene(Game &parent) : parent(parent) {} //should NOT do anything apart from being called
		virtual ~Scene() {}
		virtual bool init() = 0; //should call loadResources()
		virtual void update(float deltaTime) = 0; //called by parent on parent.update()
		virtual void draw() const = 0; //called by parent on parent.draw()
		virtual void onKeyPressed(float deltaTime, const sf::Keyboard::Key &key) = 0; //called by parent on parent.update()
		virtual void onKeyDown(float deltaTime, const sf::Keyboard::Key &key) = 0; //called by parent on parent.update()
		virtual void onKeyReleased(float deltaTime, const sf::Keyboard::Key &key) = 0; //called by parent on parent.update()
		virtual void onMouseButtonPressed(float deltaTime, const sf::Event& event) = 0; //called by parent on parent.update()
		virtual void onMouseMoved(float deltaTime, const sf::Event& event) = 0;
		virtual void onClose() = 0; //close scene-wide stuff
		
	protected:
		Game& parent;
		virtual bool loadResources() = 0; //load stuff with parent.resources
};

#endif // SCENE_HPP
