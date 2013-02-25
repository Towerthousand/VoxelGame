#ifndef TOOLS_H
#define TOOLS_H

#define GL_GLEXT_PROTOTYPES 1
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <cmath>
#include <exception>

extern int SCRWIDTH; //1366
extern int SCRHEIGHT; //768
extern bool WINDOWFOCUS;

inline float dot(const sf::Vector3f& a, const sf::Vector3f& b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline float norm(sf::Vector3f v) {
	return sqrt(dot(v, v));
}

inline void normalize(sf::Vector3f& v) {
	v /= norm(v);
}

#define WINDOW_TITLE "VoxelGame"
#define CONTEXT_SETTINGS_OPENGL sf::ContextSettings(32,32,0,4,3)
#define CHUNKWIDTH 16 //blocks in x and z coords
#define CHUNKHEIGHT 128 //blocks in y coords
#define WORLDSIZE 20 //worldsize in CHUNKSxCHUNKS
#define SEALEVEL 64
#define PLAYER_HEIGHT 0
#define DEG_TO_RAD ((2*M_PI)/360.0)

#endif
