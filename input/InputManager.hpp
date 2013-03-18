#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP
#include "tools.hpp"

class InputManager {
	public:
		InputManager();
		~InputManager();
		void pressKey(sf::Keyboard::Key key);
		void releaseKey(sf::Keyboard::Key key);
		void update();

		bool keyPressed(sf::Keyboard::Key key);
		bool keyDown(sf::Keyboard::Key key);
		bool keyReleased(sf::Keyboard::Key key);

		std::vector<bool> pressed; //is being pressed
		std::vector<bool> released; //is being releassed
		std::vector<bool> down; //is being held down
		sf::Keyboard::Key keys[101];
};

#endif // INPUTMANAGER_HPP
