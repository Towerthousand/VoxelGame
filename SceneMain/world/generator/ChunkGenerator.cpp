#include "ChunkGenerator.hpp"
#include "../Chunk.hpp"
#include "../../SceneMain.hpp"

ChunkGenerator::ChunkGenerator(SceneMain* scene) : parentScene(scene) {
}

ChunkGenerator::~ChunkGenerator() {
}

Chunk* ChunkGenerator::getChunk(int x, int y, int z) {
	Chunk* chunk = new Chunk(x,y,z,parentScene);
	bool onDisk = false;
	if (onDisk) {
		//copy contents from disc into cubes...
	}
	else {
		//generate new terrain
	}
	return chunk;
}
