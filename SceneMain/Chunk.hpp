#ifndef CHUNK_HPP
#define CHUNK_HPP
#include "tools.hpp"

class World;
class Chunk { //16*16*16
	public:
		Chunk(int x, int y, int z, World &world);
		~Chunk();
		//main
		void update(float deltaTime);
		void draw() const;

		bool outOfView;
		bool markedForRedraw;
		std::vector<std::vector<std::vector<Cube> > > cubes;
	private:
		//Getters & consultors
		bool getOutOfBounds(int x, int y, int z) const;
		Cube getCube(int x, int y, int z) const;
		bool getSkyAccess(int x,int y, int z) const;

		void pushCubeToArray(int x, int y, int z, int cubeID);
		void makeVbo();

		int XPOS; //x pos of chunk inside world matrix
		int YPOS; //y pos of chunk inside world matrix
		int ZPOS; //z pos of chunk inside world matrix
		int VBOID;
		World& parentWorld;
		static const int textureIndexes[9][6];
		std::vector<Vertex> renderData;
};

#endif // CHUNK_HPP
