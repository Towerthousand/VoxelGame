#include "World.hpp"
#include "Chunk.hpp"

World::World() {
	chunks = std::vector<std::vector<Chunk*> > //init chunks to 0, will never be used like this.
			 (WORLDSIZE,std::vector<Chunk*>
			 (WORLDSIZE,NULL));
}

World::~World() {
	for (int x = 0; x < WORLDSIZE; ++x) {
		for (int z = 0; z < WORLDSIZE; ++z) {
			delete chunks[x][z];
			chunks[x][z] = NULL;
		}
	}
}

int World::getCubeAbs(int x, int y, int z) const {
	if (uint(x/CHUNKWIDTH) >= chunks.size() || uint(z/CHUNKWIDTH) >= chunks[0].size() //out of chunk grid (positive side)
		|| x%CHUNKWIDTH < 0 || z%CHUNKWIDTH < 0) //out of the chunk grid (negative side)
		return 0;
	else return chunks[x/CHUNKWIDTH][z/CHUNKWIDTH]->cubes[x%CHUNKWIDTH][y][z%CHUNKWIDTH];
}

void World::setCubeAbs(int x, int y, int z, int id) {
	chunks[x/CHUNKWIDTH][z/CHUNKWIDTH]->setCube(x%CHUNKWIDTH,y,z%CHUNKWIDTH,id);
}

void World::regenChunk(int x, int z, int seed) {
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

void World::drawWireCube(sf::Vector3f pos) {

}

void World::update(float deltaTime) {
	for (int x = 0; x < WORLDSIZE; ++x)  {
		for (int z = 0; z < WORLDSIZE; ++z) {
			chunks[x][z]->update(deltaTime);
		}
	}
}

