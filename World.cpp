#include "World.hpp"

World::World() {
	chunks = std::vector<std::vector<Chunk*> > //init chunks to 0, will never be used like this.
			 (WORLDSIZE,std::vector<Chunk*>
			 (WORLDSIZE,NULL));
}

World::~World() {
}

int World::getCubeAbs(int x, int y, int z) const {
	if (uint(x/CHUNKWIDTH) >= chunks.size() || uint(z/CHUNKWIDTH) >= chunks[0].size() || x%CHUNKWIDTH < 0 || z%CHUNKWIDTH < 0)
		return 0;
	else return chunks[x/CHUNKWIDTH][z/CHUNKWIDTH]->cubes[x%CHUNKWIDTH][y][z%CHUNKWIDTH];
}

void World::setCubeAbs(int x, int y, int z, int id) {
	chunks[x/CHUNKWIDTH][z/CHUNKWIDTH]->setCube(x%CHUNKWIDTH,y,z%CHUNKWIDTH,id);
}

void World::populateChunk(int x, int z, int seed) {
	if (chunks[x][z] != NULL)
		delete chunks[x][z];
	chunks[x][z] = new Chunk(x,z,seed,*this);
}

void World::draw() const {
	for (int x = 0; x < WORLDSIZE; ++x)  {
		for (int z = 0; z < WORLDSIZE; ++z) {
			chunks[x][z]->draw();
		}
	}
}

void World::update(float deltaTime) {
	for (int x = 0; x < WORLDSIZE; ++x)  {
		for (int z = 0; z < WORLDSIZE; ++z) {
			chunks[x][z]->update(deltaTime);
		}
	}
}

