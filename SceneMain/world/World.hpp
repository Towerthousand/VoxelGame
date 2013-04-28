#ifndef WORLD_HPP
#define WORLD_HPP
#include "tools.hpp"
#include "generator/ChunkGenerator.hpp"

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
class World {
	public:
		World(SceneMain* parentScene, Player* player);
		~World();
		//generators
		bool loadInitialChunks();

		//Getters and setters
		bool getOutOfBounds(int x, int y, int z) const;
		Cube getCube(int x, int y, int z) const;
		std::pair<vec3i,vec3i> getCoords(int x, int y, int z) const;
		vec2i getSkyCoords(int x, int z) const;
		Cube getCubeRaw(int x, int y, int z) const;
		int getSkylightLevel(int x, int z) const;
		bool getSkyAccess(int x, int y, int z) const;
		void setCubeID(int x, int y, int z, unsigned char ID);
		void setCubeLight(int x, int y, int z, unsigned char light);

		//main
		void draw() const;
		void update(float deltaTime);
		void traceView(const Player *playerCam, float tMax);

		//this should go in graphicutils or something
		void drawWireCube(const vec3f& pos) const;

		//lololol shouldn't be public, fix it broh
		void calculateLight(vec3i source, vec2i radius);

		bool playerTargetsBlock;
		vec3f targetedBlock;
		vec3f last;

	private:
		void processCubeLighting(const vec3i& source, const vec3i& offset, std::vector<vec3i> &queue);
		void updateStuff(float deltaTime);

		SceneMain* parentScene;
		Player* player;
		ChunkGenerator chunkGen;
		std::vector<std::vector<std::vector<Chunk*> > > chunks;
		std::vector<std::vector<int> > skyValues;
		float updateStuffTimer;
		static const int vertexPoints[8][3];
		static const int indexes[24];
};

#endif // WORLD_HPP
