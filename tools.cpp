#include "Game.hpp"

//extern'd in tools.hpp
int SCRWIDTH = sf::VideoMode::getFullscreenModes()[0].width;
int SCRHEIGHT = sf::VideoMode::getFullscreenModes()[0].height;
bool WINDOWFOCUS = false;
int WORLDHEIGHT = 0;
int WORLDWIDTH = 0;
sf::Clock GLOBALCLOCK;

//implement random functions here
std::string toString(float num) {
	std::stringstream ss;
	ss << num;
	return ss.str();
}
