#ifndef TOOLS_HPP
#define TOOLS_HPP

#define GL_GLEXT_PROTOTYPES 1
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <queue>
#include <utility>
#include <random>
#include <list>
#include <functional>
#include <sys/stat.h>
#include <thread>
#include <mutex>
#include "BudMath.hpp"

using namespace math;

struct Vertex {
		Vertex(short vx = 0, short vy = 0, short  vz = 0,
			   short tx = 0, short ty = 0,
			   unsigned char cr = 255, unsigned char cg = 255, unsigned char cb = 255, unsigned char ca = 255) :
			vx(vx), vy(vy), vz(vz),
			tx(tx), ty(ty),
			cr(cr), cg(cg), cb(cb), ca(ca) {}
		short
		vx,vy,vz,
		tx,ty;
		unsigned char
		cr,cg,cb,ca;
};

struct Cube {
		Cube (unsigned char ID, unsigned char light) : ID(ID), light(light) {}
		unsigned char ID;
		unsigned char light;
};

//prototype random functions here (define in tools.cpp). Inlines go here too
inline void outLog(const std::string& msg) {
	std::cout << msg << std::endl;
}
std::string toString(float num);

#define WINDOW_TITLE "VoxelGame"
#define CONTEXT_SETTINGS_OPENGL sf::ContextSettings(32,32,0,3,0)

#define CHUNKSIZE_POW2 4
#define CHUNKSIZE 16 //CHUNKSIZE << CHUNKSIZE_POW2
#define CHUNKSIZE_MASK 15 //CHUNKSIZE -1

#define WORLDWIDTH 32
#define WORLDWIDTH_MASK 31 //WORLDWIDTH -1

#define WORLDHEIGHT 16
#define WORLDHEIGHT_MASK 15 //WORLDHEIGHT -1

#define PLAYER_HEIGHT -1.8
#define UPDATERADIUS 17.0f //How many potential light blocks does changing a block affect? Not taking into account skylight.
#define MAXLIGHT 20
#define MINLIGHT 3
#define DEG_TO_RAD ((2*M_PI)/360.0f)
#define FOV 60.0f //degrees
#define ZNEAR 0.01f
#define ZFAR 1000.0f
#define TEXSIZE 8
#define GRAVITY -40
#define MAX_TEXTURES 1
extern int SCRWIDTH;
extern int SCRHEIGHT;
extern bool WINDOWFOCUS;
extern sf::Clock GLOBALCLOCK;

#endif // TOOLS_HPP
