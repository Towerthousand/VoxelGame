#ifndef TOOLS_HPP
#define TOOLS_HPP

//OpenGL (Open Graphics Library)
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h> //core

//SFML (Simple Fast Media Library)
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//STL (Standard Toolkit Library)
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
#include <algorithm>

//GLM (openGL Math)
#include <glm/glm.hpp> // vec3, vec4, ivec4, mat4 (core)
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp> // value_ptr
#include <glm/gtc/noise.hpp> //noise

//math typedefs
typedef glm::detail::tvec4<int> vec4i;
typedef glm::detail::tvec4<float> vec4f;
typedef glm::detail::tvec4<double> vec4d;
typedef glm::detail::tvec4<uint> vec4ui;
typedef glm::detail::tvec3<int> vec3i;
typedef glm::detail::tvec3<float> vec3f;
typedef glm::detail::tvec3<double> vec3d;
typedef glm::detail::tvec3<uint> vec3ui;
typedef glm::detail::tvec2<int> vec2i;
typedef glm::detail::tvec2<float> vec2f;
typedef glm::detail::tvec2<double> vec2d;
typedef glm::detail::tvec2<uint> vec2ui;
typedef glm::detail::tmat2x2<float> mat2f;
typedef glm::detail::tmat3x3<float> mat3f;
typedef glm::detail::tmat4x4<float> mat4f;
typedef glm::detail::tmat2x2<double> mat2d;
typedef glm::detail::tmat3x3<double> mat3d;
typedef glm::detail::tmat4x4<double> mat4d;

//GLM provides no overload for operator< in vectors and matrices.
//This is the Functor to use when making a templated container for
//such classes (i.e. std::priority_queue, std::set)
struct FunctorCompare{
		bool operator()(const std::pair<float,vec3i> &a, const std::pair<float,vec3i> &b) {
			return (a.first < b.first);
		}
		bool operator()(const vec3i &a, const vec3i &b) {
			if(a.x == b.x) {
				if (a.y == b.y)
					return (a.z < b.z);
				return (a.y < b.y);
			}
			return (a.x < b.x);
		}
};

//Vertex of a voxel cube. Normals will be calculated during shading if needed
struct Vertex {
		Vertex(short vx = 0, short vy = 0, short  vz = 0,
			   short tx = 0, short ty = 0,
			   unsigned char cr = 255, unsigned char cg = 255, unsigned char cb = 255, unsigned char ca = 255) :
			vx(vx), vy(vy), vz(vz),
			tx(tx), ty(ty),
			cr(cr), cg(cg), cb(cb), ca(ca) {}
		char vx,vy,vz,shit;
		short tx,ty;
		unsigned char
		cr,cg,cb,ca;
};

//world voxel
//light = 0 for solid blocks
//MINLIGHT <= light <= MAXLIGHT for non-solid blocks
struct Cube {
		Cube (unsigned char ID, unsigned char light) : ID(ID), light(light) {}
		unsigned char ID;
		unsigned char light;
};

//prototype misc functions here (define in tools.cpp) and inlines.
inline void outLog(const std::string& msg) {std::cout << msg << std::endl;}
std::string toString(float num);

//defines (global settings and constants)
#define WINDOW_TITLE "VoxelGame"
#define CONTEXT_SETTINGS_OPENGL sf::ContextSettings(32,32,0,4,2)
#define DEG_TO_RAD ((2*M_PI)/360.0f)

//world size
#define CHUNKSIZE_POW2 4 //CHUNKSIZE must be 1 << CHUNKSIZE_POW2
#define CHUNKSIZE 16 //in voxels
#define CHUNKSIZE_MASK 15 //CHUNKSIZE -1
#define WORLDSIZE 32 //in chunks
#define WORLDSIZE_MASK 31 //WORLDWIDTH -1

//light settings
#define MAXLIGHT 16
#define MINLIGHT 3
#define UPDATERADIUS 13.0f //How many potential light blocks does changing a block affect? Not taking into account skylight.

//frustum settings
#define FOV 60.0f //degrees
#define ZNEAR 0.01f
#define ZFAR 1000.0f

//texture settings
#define MAX_TEXTURES 1 //max number of currently used textures
#define TEXSIZE 8 //size of block textures

//physics
#define GRAVITY -40

//global variables
extern int SCRWIDTH;
extern int SCRHEIGHT;
extern bool WINDOWFOCUS;
extern sf::Clock GLOBALCLOCK;

#endif // TOOLS_HPP
