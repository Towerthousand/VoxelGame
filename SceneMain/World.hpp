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
class Camera;
class World {
	public:
		World();
		~World();
		//generators
		void regenChunk(int x, int y, int z, int seed);

		//Getters and setters
		bool outOfBounds(int x, int y, int z);
		Cube getCubeAbs(int x, int y, int z) const;
		void setCubeAbs(int x, int y, int z, Cube c);

		//main
		void draw() const;
		void update(float deltaTime, const Camera &camera);
		void traceView(const Camera& player, float tMax);
        void calculateLight(sf::Vector3i source, sf::Vector3i radius);

		bool playerTargetsBlock;
		int chunksDrawn;
		sf::Vector3f targetedBlock;
		sf::Vector3f last;
		std::vector<std::vector<std::vector<Chunk*> > > chunks;

	private:
        float grassTimer;
		void drawWireCube(const sf::Vector3f& pos) const;
        void processCubeLighting(sf::Vector3i source, sf::Vector3i offset, std::queue<sf::Vector3i> &queue);
        void pdateGrass(float deltaTime);

		static const int vertexPoints[8][3];
		static const int indexes[24];
};

#endif // WORLD_HPP
