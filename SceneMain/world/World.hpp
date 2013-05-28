#ifndef WORLD_HPP
#define WORLD_HPP
#include "tools.hpp"
#include "generator/ChunkGenerator.hpp"
#include "Octree.h"

//chunk grid coords:
//0 <= x < WORLDWIDTH*CHUNKSIZE
//0 <= z < WORLDWIDTH*CHUNKSIZE
//0 <= y < WORLDHEIGHT*CHUNKSIZE
//
//infdev coordinates (Work In Progress):
//MIN_INT <= x,z <= MAX_INT
//0 <= y < WORLDHEIGHT*CHUNKSIZE

class Chunk;
class SceneMain;
class Player;
class skyValue;
class World {
	public:
		World(SceneMain* parentScene, Player* player);
		~World();

		//Getters and setters
		bool getOutOfBounds(int x, int y, int z) const;
		Cube getCube(int x, int y, int z) const;
		void setCubeID(int x, int y, int z, unsigned char ID);
		void setCubeLight(int x, int y, int z, unsigned char light);

		//main
		void draw() const;
		void update(float deltaTime);
		void traceView(const Player *playerCam, float tMax);

		//this should go in graphic utils or something
		void drawWireCube(const vec3f& pos) const;

		bool playerTargetsBlock;
		vec3f targetedBlock;
		vec3f last;

	private:
        Octree octree;

		//Lighting
		void calculateLight(vec3i source, int radius);
		void calculateLightManhattan(vec3i source, int radius);

		//BFS Helper functions
		void processCubeLighting(const vec3i& source, const vec3i& offset, std::vector<vec3i> &queue);

		SceneMain* parentScene;
		Player* player;
		ChunkGenerator chunkGen;
		static const int vertexPoints[8][3];
		static const int indexes[24];
};

#endif // WORLD_HPP
