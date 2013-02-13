#ifndef WORLD_HPP
#define WORLD_HPP
#include "tools.hpp"

class Chunk;
class World
{
	public:
		World();
		~World();
		int getCubeAbs(int x, int y, int z) const;
		void setCubeAbs(int x, int y, int z, int id);
		void regenChunk(int x, int z, int seed);
		void draw() const;
		void update(float deltaTime);

	private:
		std::vector<std::vector<Chunk*> > chunks;
};

#endif // WORLD_HPP
