#include "ChunkGenerator.hpp"
#include "../Chunk.hpp"
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

ChunkGenerator::ChunkGenerator(SceneMain* scene, int seed, std::vector<std::vector<std::vector<Chunk*> > >* chunkStorage) :
	parentScene(scene),
	entry(NULL),
	chunkStorage(chunkStorage),
	endChunkThread(false) {
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
}

ChunkGenerator::~ChunkGenerator() {
	delete entry; //will delete all child node functions recursively into the function tree
}

Chunk* ChunkGenerator::getChunk(int x, int y, int z) { //chunkgrid coords
	Chunk* chunk = new Chunk(x,y,z,parentScene);
	ID3Data data = entry->getID3Data(x*CHUNKSIZE,y*CHUNKSIZE,z*CHUNKSIZE,CHUNKSIZE,CHUNKSIZE+5,CHUNKSIZE);
	for (int i = 0; i < CHUNKSIZE; ++i)
		for (int j = 0; j < CHUNKSIZE; ++j)
			for (int k = 0; k < CHUNKSIZE; ++k)
				chunk->cubes[i][j][k] = Cube(data[i][j][k],0);
	return chunk;
}

void ChunkGenerator::queueChunk(int x, int y, int z) { //chunkgrid coords
	vec3f chunkIndex; //this is the position of the chunk in the chunk matrix
	chunkIndex.x = (z < 0 ? ((x%WORLDWIDTH) + WORLDWIDTH)%WORLDWIDTH : x%WORLDWIDTH);
	chunkIndex.y = (y < 0 ? ((y%WORLDWIDTH) + WORLDWIDTH)%WORLDWIDTH : y%WORLDWIDTH);
	chunkIndex.z = (z < 0 ? ((z%WORLDWIDTH) + WORLDWIDTH)%WORLDWIDTH : z%WORLDWIDTH);
	//1. delete the chunk that is in the place of the new chunk and assign pointer to null
	if((*chunkStorage)[chunkIndex.x][chunkIndex.y][chunkIndex.z] != NULL) {
		delete (*chunkStorage)[chunkIndex.x][chunkIndex.y][chunkIndex.z];
		(*chunkStorage)[chunkIndex.x][chunkIndex.y][chunkIndex.z] = NULL;
	}
	//2. queue new chunk

}

void ChunkGenerator::threadedChunkManagement(std::vector<std::vector<std::vector<Chunk*> > >* storage,std::queue<vec3i>* queue, bool* ended) {
	bool work;
	vec3i nextChunk;
	while(true) {
		//1. CHECK FOR END OF THREAD OR NEW CHUNKS TO PROCESS
		//turn mutex on
		if(*ended) {
			//turn mutex off
			return;
		}
		if(!queue->empty()) {
			work = true;
			nextChunk = queue->front();
			queue->pop();
		}
		//turn mutex off
		if(work) {
			//2. CREATE NEW CHUNK
			Chunk* newChunk;
			vec3i chunkIndex; //this is the position of the chunk in the chunk matrix
			chunkIndex.x = (nextChunk.x < 0 ? ((nextChunk.x%WORLDWIDTH) + WORLDWIDTH)%WORLDWIDTH : nextChunk.x%WORLDWIDTH);
			chunkIndex.y = (nextChunk.y < 0 ? ((nextChunk.y%WORLDHEIGHT) + WORLDHEIGHT)%WORLDHEIGHT : nextChunk.y%WORLDHEIGHT);
			chunkIndex.z = (nextChunk.z < 0 ? ((nextChunk.z%WORLDWIDTH) + WORLDWIDTH)%WORLDWIDTH : nextChunk.z%WORLDWIDTH);
			bool onDisk = false;
			if (onDisk) {
				//copy contents from disc into cubes...
			}
			else {
				ID3Data data = entry->getID3Data(x*CHUNKSIZE,y*CHUNKSIZE,z*CHUNKSIZE,CHUNKSIZE,CHUNKSIZE+5,CHUNKSIZE);
				for (int i = 0; i < CHUNKSIZE; ++i)
					for (int j = 0; j < CHUNKSIZE; ++j)
						for (int k = 0; k < CHUNKSIZE; ++k)
							newChunk->cubes[i][j][k] = Cube(data[i][j][k],0);
			}
			//3. REPLACE OLD POINTER
			//turn mutex on
			if(storage[chunkIndex.x][chunkIndex.y][chunkIndex.z] != NULL) {
				outLog("#ERROR -ChunkManager- Memory has not been properly freed before asigning new chunk");
				delete newChunk;
			}
			else
				storage[chunkIndex.x][chunkIndex.y][chunkIndex.z] = newChunk;
			//turn mutex off
			work = false;
		}
	}
}