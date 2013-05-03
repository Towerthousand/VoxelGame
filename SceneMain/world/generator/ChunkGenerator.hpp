#ifndef WORLDGENERATOR_HPP
#define WORLDGENERATOR_HPP
#include "tools.hpp"

class Chunk;
class SceneMain;
class FunctionTerrain;
class ChunkGenerator {
	public:
		ChunkGenerator(SceneMain* scene, int seed);
		~ChunkGenerator();

		bool queueChunk(vec3i chunk); //chunkgrid coords
		void threadedChunkManagement();

		static std::mutex inputQueueMutex;
		static std::mutex outputQueueMutex;

	private:
		SceneMain* parentScene;
		std::mt19937 generator; //Mersenne twister with nice configuration

		FunctionTerrain* entry; //root function for the generation tree
		std::set<vec3i> chunksToLoad;
		std::list<Chunk*> chunksLoaded;
};

#endif // WORLDGENERATOR_HPP
