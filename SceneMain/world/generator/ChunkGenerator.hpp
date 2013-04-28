#ifndef WORLDGENERATOR_HPP
#define WORLDGENERATOR_HPP
#include "tools.hpp"

class Chunk;
class SceneMain;
class FunctionTerrain;
class ChunkGenerator {
	public:
		ChunkGenerator(SceneMain* scene, int seed, std::vector<std::vector<std::vector<Chunk*> > >* chunkStorage);
		~ChunkGenerator();

		Chunk* getChunk(int x, int y, int z); //chunkgrid coords
		bool queueChunk(int x, int y, int z); //chunkgrid coords
		void threadedChunkManagement(int x, int y, int z);

	private:
		SceneMain* parentScene;
		std::mt19937 generator; //Mersenne twister with nice configuration

		FunctionTerrain* entry; //root function for the generation tree
		std::vector<std::vector<std::vector<Chunk*> > >* chunkStorage;
		bool endChunkThread;
};

#endif // WORLDGENERATOR_HPP
