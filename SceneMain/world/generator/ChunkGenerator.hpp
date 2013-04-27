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
		void queueChunk(int x, int y, int z); //chunkgrid coords
		void threadedChunkManagement(std::vector<std::vector<std::vector<Chunk*> > >* storage,std::queue<vec3i>* queue, bool* ended);

	private:
		SceneMain* parentScene;
		std::mt19937 generator; //Mersenne twister with nice configuration

		FunctionTerrain* entry; //root function for the generation tree
		std::vector<std::vector<std::vector<Chunk*> > >* chunkStorage;
		std::queue<vec3i> chunkQueue;
		bool endChunkThread;
};

#endif // WORLDGENERATOR_HPP
