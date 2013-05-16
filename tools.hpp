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
// vec3, vec4, ivec4, mat4
#include <glm/glm.hpp>
// translate, rotate, scale, perspective
#include <glm/gtc/matrix_transform.hpp>
// value_ptr
#include <glm/gtc/type_ptr.hpp>

typedef glm::detail::tvec4<int> vec4i;
typedef glm::detail::tvec4<float> vec4f;
typedef glm::detail::tvec4<uint> vec4ui;
typedef glm::detail::tvec3<int> vec3i;
typedef glm::detail::tvec3<float> vec3f;
typedef glm::detail::tvec3<uint> vec3ui;
typedef glm::detail::tvec2<int> vec2i;
typedef glm::detail::tvec2<float> vec2f;
typedef glm::detail::tvec2<uint> vec2ui;
typedef glm::detail::tmat2x2<float> mat2f;
typedef glm::detail::tmat3x3<float> mat3f;
typedef glm::detail::tmat4x4<float> mat4f;

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
