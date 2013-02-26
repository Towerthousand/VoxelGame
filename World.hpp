#ifndef WORLD_HPP
#define WORLD_HPP
#include "tools.hpp"


//chunk grid coords:
//0 <= x < WORLDSIZE*CHUNKWIDTH
//0 <= z < WORLDSIZE*CHUNKWIDTH
//0 <= x < CHUNKHEIGHT
//
//infdev coordinates (Work In Progress):
//MIN_INT <= x,y,z <= MAX_INT
//
//Conversion must be done by Chunk::getCube

class Chunk;
class Camera;
class World {
	public:
		World();
		~World();
		int getCubeAbs(int x, int y, int z) const;
		void setCubeAbs(int x, int y, int z, int id);
		void regenChunk(int x, int z, int seed);
		void draw() const;
		void drawWireCube(sf::Vector3f pos) const;
		void update(float deltaTime);
		void traceView(const Camera& player, float tMax);

		bool playerTargetsBlock;
		sf::Vector3f targetedBlock;
		sf::Vector3f last;
		static const int vertexPoints[8][3];
		static const int indexes[24];

	private:
		std::vector<std::vector<Chunk*> > chunks;
};

#endif // WORLD_HPP
