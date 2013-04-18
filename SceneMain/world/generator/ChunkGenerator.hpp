#ifndef WORLDGENERATOR_HPP
#define WORLDGENERATOR_HPP
#include "tools.hpp"

class Chunk;
class SceneMain;
class ChunkGenerator {
	public:
		ChunkGenerator(SceneMain * scene);
		~ChunkGenerator();

		Chunk* getChunk(int x, int y, int z);

	private:
		SceneMain* parentScene;
};

#endif // WORLDGENERATOR_HPP
