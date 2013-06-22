#ifndef CHUNK_HPP
#define CHUNK_HPP
#include "tools.hpp"

class SceneMain;
class Chunk { //16*16*16
	public:
		Chunk(int x, int y, int z, SceneMain *scene);
		~Chunk();

		Cube &getLocal(int x, int y, int z);
		Cube &getLocal(vec3i coord);
		Cube const &getLocal(int x, int y, int z) const;
		Cube const &getLocal(vec3i coord) const;

		void initBuffer();

		//main
		void update(float deltaTime);
		void draw() const;
		void drawBoundingBox() const;

		vec3i getPos();

		bool outOfView;
		bool markedForRedraw;
		std::vector<Cube> cubes;
		int vertexCount;

		int XPOS; //x pos of chunk inside world matrix
		int YPOS; //y pos of chunk inside world matrix
		int ZPOS; //z pos of chunk inside world matrix
	private:
		//Getters & consultors
		Cube getCube(int x, int y, int z) const;

		void pushCubeToArray(short x, short y, short z, unsigned char cubeID, std::vector<Vertex> &renderData);
		void makeVbo(std::vector<Vertex> &renderData);

		int VBOID;
		mat4f modelMatrix; //precalculated, since chunks do not transform ever
		SceneMain* parentScene;
		static const int textureIndexes[9][6];
		static const float vertices[24][3]; //to draw the bounding box
};

#endif // CHUNK_HPP
