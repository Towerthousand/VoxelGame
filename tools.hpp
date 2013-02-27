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
struct Vertex {
		Vertex(float vx = 0.0, float vy = 0.0, float vz = 0.0,
			   float nx = 0.0, float ny = 0.0, float nz = 0.0,
			   float tx = 0.0, float ty = 0.0) :
			v1(vx), v2(vy), v3(vz),
			n1(nx), n2(ny), n3(nz),
			t1(tx), t2(ty)
		{}
		float v1,v2,v3,n1,n2,n3,t1,t2;
};
//prototype random functions here (define in tools.cpp)

#define WINDOW_TITLE "VoxelGame"
#define CONTEXT_SETTINGS_OPENGL sf::ContextSettings(32,32,0,3,0)
#define CHUNKWIDTH 16 //blocks in x and z coords
#define CHUNKHEIGHT 128 //blocks in y coords
#define WORLDSIZE 10 //worldsize in CHUNKSxCHUNKS
#define SEALEVEL 64
#define PLAYER_HEIGHT -2
#define DEG_TO_RAD ((2*M_PI)/360.0)

#endif
