#ifndef TOOLS_HPP
#define TOOLS_HPP

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
#include <queue>

extern int SCRWIDTH; //1366
extern int SCRHEIGHT; //768
extern bool WINDOWFOCUS;
extern int DBG_UPDATES;

struct Vertex {
		Vertex(float vx = 0.0, float vy = 0.0, float vz = 0.0,
			   float nx = 0.0, float ny = 0.0, float nz = 0.0,
               float tx = 0.0, float ty = 0.0,
               float cr = 1.0, float cg = 1.0, float cb = 1.0, float ca = 1.0) :
			vx(vx), vy(vy), vz(vz),
			nx(nx), ny(ny), nz(nz),
			tx(tx), ty(ty),
            cr(cr), cg(cg), cb(cb), ca(ca)
		{}
		float vx,vy,vz,
			  nx,ny,nz,
			  tx,ty,
              cr,cg,cb,ca;
};

struct Cube {
	Cube (int id = 0, int light = 0) : id(id), light(light) {}
    int id;
    int light;
};

//prototype random functions here (define in tools.cpp). Inlines go here too

inline void outLog(std::string msg) {
	std::cout << msg << std::endl;
}
std::string toString(float num);

#define WINDOW_TITLE "VoxelGame"
#define CONTEXT_SETTINGS_OPENGL sf::ContextSettings(32,32,0,3,0)
#define WORLDWIDTH 5 //worldwidth in CHUNKS
#define WORLDHEIGHT 5 //worldheight in CHUNKS
#define CHUNKSIZE 16
#define SEALEVEL 64
#define PLAYER_HEIGHT -1.8
#define LIGHTMAX 10.0
#define DEG_TO_RAD ((2*M_PI)/360.0)

#endif // TOOLS_HPP
