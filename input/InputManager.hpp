#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP
#include "tools.hpp"

class InputManager {
	public:
		InputManager();
		~InputManager();
		void pressKey(sf::Keyboard::Key key);
		void releaseKey(sf::Keyboard::Key key);
		void pressMouse(sf::Mouse::Button key);
		void releaseMouse(sf::Mouse::Button key);
		void update();

		bool isKeyPressed(sf::Keyboard::Key key);
		bool isKeyDown(sf::Keyboard::Key key);
		bool isKeyReleased(sf::Keyboard::Key key);
		bool isMousePressed(sf::Mouse::Button key);
		bool isMouseDown(sf::Mouse::Button key);
		bool isMouseReleased(sf::Mouse::Button key);

		std::vector<bool> keyPressed; //is being pressed
		std::vector<bool> keyReleased; //is being releassed
		std::vector<bool> keyDown; //is being held down
		sf::Keyboard::Key keys[101];

		std::vector<bool> mousePressed; //is being pressed
		std::vector<bool> mouseReleased; //is being releassed
		std::vector<bool> mouseDown; //is being held down
		sf::Mouse::Button mouseButtons[5];
};

#endif // INPUTMANAGER_HPP
