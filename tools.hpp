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
#include <cmath>
#include <queue>
#include "Math.hpp"
#include <list>

typedef sf::Vector3<float> vec3f;
typedef sf::Vector3<int> vec3i;
typedef sf::Vector2<float> vec2f;
typedef sf::Vector2<int> vec2i;

struct Vertex {
		Vertex(float vx = 0.0, float vy = 0.0, float vz = 0.0,
			   float nx = 0.0, float ny = 0.0, float nz = 0.0,
			   float tx = 0.0, float ty = 0.0,
			   float cr = 1.0, float cg = 1.0, float cb = 1.0, float ca = 1.0) :
			vx(vx), vy(vy), vz(vz), vw(0),
			nx(nx), ny(ny), nz(nz), nw(0),
			tx(tx), ty(ty), tu(0) , tv(0),
			cr(cr), cg(cg), cb(cb), ca(ca)
		{}
		float
		vx,vy,vz, vw,
		nx,ny,nz, nw,
		tx,ty,tu, tv,
		cr,cg,cb,ca;
};

struct Cube {
		Cube (short ID, short light) : ID(ID), light(light) {}
		short ID;
		short light;
};

//prototype random functions here (define in tools.cpp). Inlines go here too
inline void outLog(const std::string& msg) {
	std::cout << msg << std::endl;
}
std::string toString(float num);

#define WINDOW_TITLE "VoxelGame"
#define CONTEXT_SETTINGS_OPENGL sf::ContextSettings(32,32,0,3,0)
#define CHUNKSIZE 8
#define PLAYER_HEIGHT -1.8
#define UPDATERADIUS 13.0 //How many potential light blocks does changing a block affect? Not taking into account skylight.
						  //Usually UPDATERADIUS = MAXLIGHT-MINLIGHT
#define MAXLIGHT 15
#define MINLIGHT 1
#define DEG_TO_RAD ((2*M_PI)/360.0f)
#define FOV 60.0f //degrees
#define ZNEAR 0.01f
#define ZFAR 500.0f
#define TEXSIZE 8
extern int SCRWIDTH;
extern int SCRHEIGHT;
extern bool WINDOWFOCUS;
extern int WORLDHEIGHT;
extern int WORLDWIDTH;
extern sf::Clock GLOBALCLOCK;

#endif // TOOLS_HPP
