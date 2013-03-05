#ifndef CHUNK_HPP
#define CHUNK_HPP
#include "tools.hpp"
#include "Camera.hpp"

class World;
class Chunk { //16*16*128
	public:
		Chunk(int x, int z, int seed, World &world);
		~Chunk();

		void populate();
		void update(float deltaTime);
		void pushCubeToArray(int x, int y, int z, int cubeID);
		void pushBudToArray(int x, int y, int z, int cubeID);
		void draw() const;
		bool checkCulling(const Camera &cam);
		void makeVbo();

		void setCube(int x, int y, int z, int id);
		int getCube(int x, int y, int z) const;

		void updateGrass(float deltaTime);

		std::vector<std::vector<std::vector<int> > > cubes;
		std::vector<Vertex> renderData;
		static const int textureIndexes[4][6];

		int XPOS; //x pos of chunk inside world matrix
		int ZPOS; //z pos of chunk inside world matrix
		int SEED; //constant across chunks
		uint VBOID;

		bool markedForRedraw;
		bool outOfView;
		float grassTimer;

		World& parentWorld;
};

#endif // CHUNK_HPP
