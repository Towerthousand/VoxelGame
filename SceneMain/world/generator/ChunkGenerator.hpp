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
		void replaceQueue(std::priority_queue<std::pair<float,vec3i>, std::vector<std::pair<float,vec3i> >, FunctorCompare > newQueue);

		static std::mutex chunkMutex;
		std::map<vec3i,Chunk*,FunctorCompare> chunksLoaded;

	private:
		SceneMain* parentScene;
		std::mt19937 generator; //Mersenne twister with nice configuration

		FunctionTerrain* entry; //root function for the generation tree
		std::priority_queue<std::pair<float,vec3i>, std::vector<std::pair<float,vec3i> >, FunctorCompare > chunksToLoad;
		std::set<vec3i,FunctorCompare> chunksBeingLoaded;
};

#endif // WORLDGENERATOR_HPP
