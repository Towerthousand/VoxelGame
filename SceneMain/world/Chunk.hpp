#ifndef CHUNK_HPP
#define CHUNK_HPP
#include "tools.hpp"
#include "Octree.h"

class Octree;

class SceneMain;
class Chunk { //16*16*16
	public:
        Chunk(int x, int y, int z, int size, Octree* octree, SceneMain *scene);
		~Chunk();

		void initBuffer();

        Cube getCube(int x, int y, int z) const;

		//main
		void update(float deltaTime);
        void redraw();
        void draw() const;
		void drawBoundingBox() const;

		vec3i getPos();

		bool outOfView;

        Octree* octree;
		int vertexCount;

        int xPos; //x pos of chunk in blocks
        int yPos; //y pos of chunk in blocks
        int zPos; //z pos of chunk in blocks
        int size; //Size of chunk is 1<<size.
        SceneMain* parentScene;

	private:

		void pushCubeToArray(short x, short y, short z, unsigned char cubeID, std::vector<Vertex> &renderData);
		void makeVbo(std::vector<Vertex> &renderData);

        int vboId;
		mat4f modelMatrix; //precalculated, since chunks do not transform ever
		static const int textureIndexes[9][6];
		static const float vertices[24][3]; //to draw the bounding box
};

#endif // CHUNK_HPP
