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

		void threadedChunkManagement();
		void replaceQueue(const std::priority_queue<std::pair<float,vec3i>, std::vector<std::pair<float,vec3i> >, FunctorCompare > &newQueue);

		static std::mutex chunkMutex;
		std::queue<Chunk*> chunksLoaded;

		SceneMain* parentScene;
		std::mt19937 generator; //Mersenne twister with nice configuration

		FunctionTerrain* entry; //root function for the generation tree
		std::priority_queue<std::pair<float,vec3i>, std::vector<std::pair<float,vec3i> >, FunctorCompare > chunksToLoad;
		std::set<vec3i,FunctorCompare> chunksInProcess;
};

#endif // WORLDGENERATOR_HPP
