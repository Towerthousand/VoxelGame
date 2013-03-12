#ifndef CHUNK_HPP
#define CHUNK_HPP
#include "tools.hpp"
#include "Camera.hpp"

class World;
class Chunk { //16*16*128
	public:
		Chunk(int x, int y, int z, World &world);
		~Chunk();

		//Getters & consultors
		bool getOutOfBounds(int x, int y, int z);
		Cube getCube(int x, int y, int z);
		bool getSkyAccess(int x,int y, int z);

		//main
		void update(float deltaTime);
		void draw() const;
		bool checkCulling(const Camera &cam);

		std::vector<std::vector<std::vector<Cube> > > cubes;
		bool markedForRedraw;
		bool outOfView;
	private:
		void pushCubeToArray(int x, int y, int z, int cubeID);
        void makeVbo();

		int VBOID;
		int XPOS; //x pos of chunk inside world matrix
		int ZPOS; //z pos of chunk inside world matrix
		int YPOS; //y pos of chunk inside world matrix
		World& parentWorld;
		int SEED; //constant across chunks
		static const int textureIndexes[9][6];
		std::vector<Vertex> renderData;
};

#endif // CHUNK_HPP
