#ifndef WORLD_HPP
#define WORLD_HPP
#include "tools.hpp"

//chunk grid coords:
//0 <= x < WORLDWIDTH*CHUNKSIZE
//0 <= z < WORLDWIDTH*CHUNKSIZE
//0 <= y < WORLDHEIGHT*CHUNKSIZE
//
//infdev coordinates (Work In Progress):
//MIN_INT <= x,z <= MAX_INT
//0 <= y < WORLDHEIGHT*CHUNKSIZE

class Chunk;
class Player;
class Camera;
class World {
	public:
		World();
		~World();
		//generators
		bool loadDirbaio(const std::string filePath);
		void regenChunk(int x, int y, int z, int seed);
		void initChunkLight(int x,int y, int z);

		//Getters and setters
		bool getOutOfBounds(int x, int y, int z) const;
		Cube getCubeAbs(int x, int y, int z) const;
		void setCubeIDAbs(int x, int y, int z, short ID);
		void setCubeLightAbs(int x, int y, int z, short light);
		int getSkylightLevel(int x, int z) const;
		bool getSkyAccess(int x, int y, int z) const;

		//main
		void draw() const;
		void update(float deltaTime,Player &camera);
		void traceView(const Player &player, float tMax);

		bool playerTargetsBlock;
		int chunksDrawn;
		vec3f targetedBlock;
		vec3f last;

	private:
		void calculateLight(vec3i source, vec2i radius);
		void processCubeLighting(const vec3i& source, const vec3i& offset, std::vector<vec3i> &queue);
		void updateGrass(float deltaTime);
		void drawWireCube(const vec3f& pos) const;

		std::vector<std::vector<std::vector<Chunk*> > > chunks;
		std::vector<std::vector<int> > skyValues;
		float updateStuffTimer;
		static const int vertexPoints[8][3];
		static const int indexes[24];
};

#endif // WORLD_HPP
