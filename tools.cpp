#include "Game.hpp"

//extern'd in tools.hpp
int SCRWIDTH = 1920; //1366
int SCRHEIGHT = 1080; //768
bool WINDOWFOCUS = false;

//implement random functions here

std::string toString(float num) {
	std::stringstream ss;
	ss << num;
	return ss.str();
}
