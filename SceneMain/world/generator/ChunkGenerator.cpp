#include "ChunkGenerator.hpp"
#include "../Chunk.hpp"
#include "../../SceneMain.hpp"
#include "Function3DSimplex.hpp"
#include "Function3DSub.hpp"
#include "Function3DYcoord.hpp"
#include "Function3DAdd.hpp"
#include "Function2DConst.hpp"
#include "FunctionTerrrainVolume.hpp"
#include "FunctionTerrainOverlay.hpp"
#include "Function2DSimplex.hpp"
#include "FunctionTerrainHeightmap.hpp"
#include "FunctionTerrainJoin.hpp"

ChunkGenerator::ChunkGenerator(SceneMain* scene, int seed) :
	parentScene(scene),
	entry(NULL) {
	generator.seed(seed);
	Function3DSimplex* simplex31 = new Function3DSimplex(&generator,100,-70,70);
	Function3DSimplex* simplex32 = new Function3DSimplex(&generator,70,-50,50);
	Function3DSimplex* simplex33 = new Function3DSimplex(&generator,50,-30,30);
	Function3DSimplex* simplex34 = new Function3DSimplex(&generator,25,-20,20);
	Function3DSimplex* simplex35 = new Function3DSimplex(&generator,10,-5,5);
	Function3DAdd* add1 = new Function3DAdd(simplex31,simplex32);
	Function3DAdd* add2 = new Function3DAdd(simplex34,simplex33);
	Function3DAdd* add3 = new Function3DAdd(add1,add2);
	Function3DAdd* add4 = new Function3DAdd(add3,simplex35);
	Function3DYcoord* coord1 = new Function3DYcoord();
	Function2DConst* const1 = new Function2DConst(70);
	Function3DSub* sub1 = new Function3DSub(coord1,const1);
	Function3DSub* sub2 = new Function3DSub(add4,sub1);
	FunctionTerrrainVolume* vol1 = new FunctionTerrrainVolume(sub2,2);
	Function2DSimplex* simplex21 = new Function2DSimplex(&generator,50,90,95);
	FunctionTerrainHeightmap* terrain1 = new FunctionTerrainHeightmap(simplex21,2);
	FunctionTerrainJoin* join1 = new FunctionTerrainJoin(vol1,terrain1);
	FunctionTerrainOverlay* over1 = new FunctionTerrainOverlay(join1,1,2,4);
	FunctionTerrainOverlay* over2 = new FunctionTerrainOverlay(over1,3,1,1);
	entry = over2;

	std::thread thread(&ChunkGenerator::threadedChunkManagement,this);
	thread.detach();
	std::thread thread2(&ChunkGenerator::threadedChunkManagement,this);
	thread2.detach();
	std::thread thread3(&ChunkGenerator::threadedChunkManagement,this);
	thread3.detach();
}

ChunkGenerator::~ChunkGenerator() {
	delete entry; //will delete all child node functions recursively into the function tree
}

void ChunkGenerator::threadedChunkManagement() {
	bool found;
	while(true) {
		//Look for new chunks. If there are, generate and output to output queue. If not, sleep for a short while
		chunkMutex.lock();
		if(!chunksToLoad.empty()) {
			found = false;
			vec3i chunkPos;
			while(!chunksToLoad.empty()) {
				chunkPos = chunksToLoad.top();
				chunksToLoad.pop();
				if(chunksBeingLoaded.find(chunkPos) == chunksBeingLoaded.end() &&
				   chunksLoaded.find(chunkPos) == chunksLoaded.end()) {
					chunksBeingLoaded.insert(chunkPos); //set the chunk to load
					found = true;
					break;
				}
			}
			chunkMutex.unlock();
			if(found) {
				Chunk* newChunk = new Chunk(chunkPos.x,chunkPos.y,chunkPos.z,parentScene);
				ID3Data data = entry->getID3Data(chunkPos.x*CHUNKSIZE,chunkPos.y*CHUNKSIZE,chunkPos.z*CHUNKSIZE,CHUNKSIZE,CHUNKSIZE+5,CHUNKSIZE);
				for (int i = 0; i < CHUNKSIZE; ++i)
					for (int j = 0; j < CHUNKSIZE; ++j)
						for (int k = 0; k < CHUNKSIZE; ++k)
							newChunk->getLocal(i,j,k) = Cube(data[i][j][k],MINLIGHT);
				chunkMutex.lock();
				chunksLoaded.insert(newChunk);
				chunksBeingLoaded.erase(chunkPos);
				chunkMutex.unlock();
			}
		}
		else {
			chunkMutex.unlock();
			sf::sleep(sf::seconds(0.1));
		}
	}
}

std::mutex ChunkGenerator::chunkMutex;
