#include "Game.hpp"

//extern'd in tools.hpp
int SCRWIDTH = 1920; //1366
int SCRHEIGHT = 1080; //768
bool WINDOWFOCUS = false;
int DBG_UPDATES = 0;
int WORLDHEIGHT = 0;
int WORLDWIDTH = 0;

//implement random functions here
std::string toString(float num) {
	std::stringstream ss;
	ss << num;
	return ss.str();
}
