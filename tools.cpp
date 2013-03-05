#include "Game.hpp"

//extern'd in tools.hpp
int SCRWIDTH = 1366; //1366
int SCRHEIGHT = 768; //768
bool WINDOWFOCUS = false;

//implement random functions here

std::string toString(float num) {
	std::stringstream ss;
	ss << num;
	return ss.str();
}
