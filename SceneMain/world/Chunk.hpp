#ifndef CHUNK_HPP
#define CHUNK_HPP
#include "tools.hpp"

class World;
class Chunk { //16*16*16
	public:
		Chunk(int x, int y, int z, World *world);
		~Chunk();
		//main
		void update(float deltaTime);
		void draw() const;
		void drawBoundingBox() const;

		bool outOfView;
		bool markedForRedraw;
		std::vector<std::vector<std::vector<Cube> > > cubes;
		int vertexCount;
	private:
		//Getters & consultors
		Cube getCube(int x, int y, int z) const;

		void pushCubeToArray(short x, short y, short z, unsigned char cubeID, std::vector<Vertex> &renderData);
		void makeVbo(std::vector<Vertex> &renderData);

		int XPOS; //x pos of chunk inside world matrix
		int YPOS; //y pos of chunk inside world matrix
		int ZPOS; //z pos of chunk inside world matrix
		int VBOID;
		World* parentWorld;
		static const int textureIndexes[9][6];
};

#endif // CHUNK_HPP
