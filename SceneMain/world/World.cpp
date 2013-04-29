#include "World.hpp"
#include "Chunk.hpp"
#include "../entities/Player.hpp"
#include "../SceneMain.hpp"
#include "../../Game.hpp"

World::World(SceneMain* parentScene, Player* player) :
	playerTargetsBlock(false), targetedBlock(0,0,0),
	last(0,0,0), parentScene(parentScene), player(player),
	chunkGen(parentScene, rand()),
	chunks(WORLDWIDTH,NULL),
	updateStuffTimer(0.0) {
}

World::~World() {
	for (int x = 0; x < WORLDWIDTH; ++x)
		for (int y = 0; y < WORLDHEIGHT; ++y)
			for (int z = 0; z < WORLDWIDTH; ++z) {
				delete (*this)(x,y,z);
				(*this)(x,y,z) = NULL;
			}
}

Chunk* &World::operator()(int x, int y, int z) {
	//return chunks[x][y][z];
	return chunks[x*WORLDWIDTH*WORLDHEIGHT + y*WORLDHEIGHT + z];
}

Chunk* const &World::operator()(int x, int y, int z) const {
	//return chunks[x][y][z];
	return chunks[x*WORLDWIDTH*WORLDHEIGHT+y*WORLDHEIGHT+z];
}

bool World::loadInitialChunks() {
	sf::Clock clock;
	clock.restart();
	skyValues = std::vector<std::vector<int> >(CHUNKSIZE*WORLDWIDTH,
											   std::vector<int>(CHUNKSIZE*WORLDWIDTH,-1));
	outLog(" - Generating chunk terrain...");
	for (int x = 0; x < WORLDWIDTH; ++x)
		for (int y = 0; y < WORLDHEIGHT; ++y)
			for (int z = 0; z < WORLDWIDTH; ++z) {
				chunkGen.queueChunk(x,y,z);
			}
	outLog(" - Calculating sky levels...");
	for(int x = 0; x < CHUNKSIZE*WORLDWIDTH; ++x)
		for(int z = 0; z < CHUNKSIZE*WORLDWIDTH; ++z)
			skyValues[x][z] = getSkylightLevel(x,z);
	outLog(" - Lighting chunks...");
	calculateLight(vec3i(CHUNKSIZE*WORLDWIDTH/2,
						 CHUNKSIZE*WORLDHEIGHT/2,
						 CHUNKSIZE*WORLDWIDTH/2),
				   vec2i(WORLDWIDTH*CHUNKSIZE/2 + 1,WORLDHEIGHT*CHUNKSIZE/2 + 1));
	outLog(" - Finished lighting. Time: " + toString((float)clock.restart().asSeconds()) + " seconds");
	return true;
}

bool World::getOutOfBounds(int x, int y, int z) const{
	std::pair<vec3i,vec3i> matrixCoords = getCoords(x,y,z);
	Chunk* c = (*this)(matrixCoords.first.x,matrixCoords.first.y,matrixCoords.first.z);
	if(c == NULL)
		return true;
	if(c->XPOS != (x >> CHUNKSIZE_POW2) || c->YPOS != (y >> CHUNKSIZE_POW2) || c->ZPOS != (z >> CHUNKSIZE_POW2))
		return true;
	return false;
}

Cube World::getCube(int x, int y, int z) const {
	if (getOutOfBounds(x,y,z))
		return Cube(0,MAXLIGHT);
	return getCubeRaw(x,y,z);
}

std::pair<vec3i,vec3i> World::getCoords(int x, int y, int z) const {
	int blockIndexX = x & CHUNKSIZE_MASK;;//local coords (0 <= coord < CHUNKSIZE)
	int realChunkX = x >> CHUNKSIZE_POW2;//real coords
	int chunkIndexX = realChunkX & WORLDWIDTH_MASK; //chunkgrid coords (0 <= coord < WORLDWIDTH

	int blockIndexY = y & CHUNKSIZE_MASK;;//local coords (0 <= coord < CHUNKSIZE)
	int realChunkY = y >> CHUNKSIZE_POW2;//real coords
	int chunkIndexY = realChunkY & WORLDHEIGHT_MASK; //chunkgrid coords (0 <= coord < WORLDHEIGHT

	int blockIndexZ = z & CHUNKSIZE_MASK;;//local coords (0 <= coord < CHUNKSIZE)
	int realChunkZ = z >> CHUNKSIZE_POW2;//real coords
	int chunkIndexZ = realChunkZ & WORLDWIDTH_MASK; //chunkgrid coords (0 <= coord < WORLDWIDTH

	return std::pair<vec3i,vec3i>(vec3i(chunkIndexX,chunkIndexY,chunkIndexZ),vec3i(blockIndexX,blockIndexY,blockIndexZ));
}

vec2i World::getSkyCoords(int x, int z) const {
	int indexX = x & (WORLDWIDTH*CHUNKSIZE -1); //chunkgrid coords (0 <= coord < WORLDSIZE
	int indexZ = z & (WORLDWIDTH*CHUNKSIZE -1); //chunkgrid coords (0 <= coord < WORLDSIZE
	return vec2i(indexX,indexZ);
}


Cube World::getCubeRaw(int x, int y, int z) const {
	std::pair<vec3i,vec3i> matrixCoords = getCoords(x,y,z);
	return (*this)(matrixCoords.first.x,matrixCoords.first.y,matrixCoords.first.z)->cubes[matrixCoords.second.x][matrixCoords.second.y][matrixCoords.second.z];
}

int World::getSkylightLevel(int x, int z) const { //X and Z in cube coords
	for(int y = CHUNKSIZE*WORLDHEIGHT-1; y >= 0; --y)
		if(getCube(x,y,z).ID != 0)
			return y;
	return -1;
}

bool World::getSkyAccess(int x, int y, int z) const {
	vec2i skyCoords = getSkyCoords(x,z);
	if (y <= skyValues[skyCoords.x][skyCoords.y])
		return false;
	return true;
}

void World::setCubeID(int x, int y, int z, unsigned char ID) { //set the id, calculate light (taking into account sky level)
	if (getOutOfBounds(x,y,z))
		return;
	std::pair<vec3i,vec3i> matrixCoords = getCoords(x,y,z);
	(*this)(matrixCoords.first.x,matrixCoords.first.y,matrixCoords.first.z)->cubes[matrixCoords.second.x][matrixCoords.second.y][matrixCoords.second.z].ID = ID;
	(*this)(matrixCoords.first.x,matrixCoords.first.y,matrixCoords.first.z)->markedForRedraw = true;
	vec2i skyCoords = getSkyCoords(x,z);
	if (y > skyValues[skyCoords.x][skyCoords.y]) { // calculate "shadow" of the new block
		int previousSkyValue = skyValues[skyCoords.x][skyCoords.y];
		skyValues[skyCoords.x][skyCoords.y] = getSkylightLevel(x,z);
		calculateLight(vec3i(x,
							 ((y - previousSkyValue)/2) + previousSkyValue - UPDATERADIUS/2,
							 z),
					   vec2i(UPDATERADIUS,std::max(UPDATERADIUS,(y - previousSkyValue)/2 + UPDATERADIUS/2 + 4)));
	}
	else if (y == skyValues[skyCoords.x][skyCoords.y]) { //propagate skylight downwards
		skyValues[skyCoords.x][skyCoords.y] = getSkylightLevel(x,z);
		calculateLight(vec3i(x,
							 ((y - skyValues[skyCoords.x][skyCoords.y])/2) + skyValues[skyCoords.x][skyCoords.y]  - UPDATERADIUS/2,
							 z),
					   vec2i(UPDATERADIUS,std::max(UPDATERADIUS,(y - skyValues[skyCoords.x][skyCoords.y])/2 + UPDATERADIUS/2 + 4)));
	}
	else { //just calculate the surrounding blocks, since wer'e not changing sky level
		calculateLight(vec3i(x,y,z),vec2i(UPDATERADIUS,UPDATERADIUS));
	}
}

void World::setCubeLight(int x, int y, int z, unsigned char light) { //set the light, mark for redraw
	if (getOutOfBounds(x,y,z))
		return;
	std::pair<vec3i,vec3i> matrixCoords = getCoords(x,y,z);
	(*this)(matrixCoords.first.x,matrixCoords.first.y,matrixCoords.first.z)->cubes[matrixCoords.second.x][matrixCoords.second.y][matrixCoords.second.z].light = light;
	(*this)(matrixCoords.first.x,matrixCoords.first.y,matrixCoords.first.z)->markedForRedraw = true;
}

void World::draw() const {
	parentScene->chunksDrawn = 0;
	//empty culling
	for (int x = 0; x < WORLDWIDTH; ++x)
		for (int y = 0; y < WORLDHEIGHT; ++y)
			for (int z = 0; z < WORLDWIDTH; ++z)
				if((*this)(x,y,z) != NULL) {
					vec3f center = (*this)(x,y,z)->getPos() + vec3i(CHUNKSIZE/2);
					if ((*this)(x,y,z)->vertexCount == 0)
						(*this)(x,y,z)->outOfView = true;
					else if (abs((player->pos - center).module()) < CHUNKSIZE*5)
						(*this)(x,y,z)->outOfView = false;
					else
						(*this)(x,y,z)->outOfView = !player->insideFrustum(center,sqrt(3*((CHUNKSIZE/2)*(CHUNKSIZE/2))));
				}

	//do occlusion culling here!
	std::priority_queue<std::pair<float,Chunk*> > queryList; //chunks to be queried, ordered by distance

	//sort by distance
	float dist = 0;
	for(int x = 0; x < WORLDWIDTH; ++x)
		for(int y = 0; y < WORLDHEIGHT; ++y)
			for(int z = 0; z < WORLDWIDTH; ++z)
				if((*this)(x,y,z) != NULL)
					if(!(*this)(x,y,z)->outOfView) {
						dist = ((*this)(x,y,z)->getPos() + vec3f(CHUNKSIZE/2) - parentScene->player->pos).module();
						queryList.push(std::pair<float,Chunk*>(-dist,(*this)(x,y,z)));
					}

	int layers = 10;
	int chunksPerLayer = queryList.size()/layers + int(queryList.size()%layers > 0); //chunks per pass
	//first layer is always drawn
	for(int i = 0; i < chunksPerLayer && queryList.size() > 0; i++) {
		std::pair<float,Chunk*> c = queryList.top();
		queryList.pop();
		c.second->draw();
		++parentScene->chunksDrawn;
	}
	//Query other layers
	for(int currLayer = 1; currLayer < layers && queryList.size() > 0; ++currLayer) {
		std::vector<GLuint> queries(chunksPerLayer,0);
		std::vector<Chunk*> chunkPointers(chunksPerLayer,NULL);

		//disable rendering state
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);
		glDisable(GL_LIGHTING);

		//generate and send the queries
		int queriesSent = 0;
		glGenQueries(chunksPerLayer, &queries[0]);
		for (int i = 0; i < chunksPerLayer && queryList.size() > 0; ++i) {
			Chunk* currChunk = queryList.top().second;
			chunkPointers[i] = currChunk;
			queryList.pop();

			glBeginQuery(GL_ANY_SAMPLES_PASSED,queries[i]);
			currChunk->drawBoundingBox();
			glEndQuery(GL_ANY_SAMPLES_PASSED);
			++queriesSent;
		}

		//enable rendering state
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glDisable(GL_LIGHTING);

		//collect query results
		for (int i = 0; i < queriesSent; ++i) {
			//if we have pending query, get result
			GLint seen;
			glGetQueryObjectiv(queries[i],GL_QUERY_RESULT, &seen);
			//if seen, draw it
			if (seen > 0) {
				chunkPointers[i]->draw();
				++parentScene->chunksDrawn;
			}
			else
				chunkPointers[i]->drawBoundingBox();
		}
		//delete the queries
		glDeleteQueries(queries.size(),&queries[0]);
	}
}

void World::update(float deltaTime) {
	//updateStuff(deltaTime);
	vec2i playerChunkPos = vec2i(std::floor(player->pos.x),std::floor(player->pos.z))/CHUNKSIZE;
	int count = 0;
	for (int x = -WORLDWIDTH/2; x < WORLDWIDTH/2; ++x)
		for (int y = 0; y < WORLDHEIGHT; ++y)
			for (int z = -WORLDWIDTH/2; z < WORLDWIDTH/2; ++z){
				if(count < 10)
					if(chunkGen.queueChunk(playerChunkPos.x+x,y,playerChunkPos.y+z))
						++count;
			}
	traceView(player,10);
	for (int x = 0; x < WORLDWIDTH; ++x)
		for (int y = 0; y < WORLDHEIGHT; ++y)
			for (int z = 0; z < WORLDWIDTH; ++z)
				if((*this)(x,y,z) != NULL)
					if ((*this)(x,y,z)->markedForRedraw == true)
						(*this)(x,y,z)->update(deltaTime);
}

//Based on: Fast Voxel Traversal Algorithm for Ray Tracinge
//By: John Amanatides et al.
//Implemented by Jordi "BuD" Santiago Provencio
void World::traceView(const Player *playerCam, float tMax) {
	if (!getOutOfBounds(floor(playerCam->camPos.x),floor(playerCam->camPos.y),floor(playerCam->camPos.z)) &&
		getCube(floor(playerCam->camPos.x),floor(playerCam->camPos.y),floor(playerCam->camPos.z)).ID != 0) {
		playerTargetsBlock = true;
		targetedBlock = vec3f(floor(playerCam->camPos.x),floor(playerCam->camPos.y),floor(playerCam->camPos.z));
		return;
	}

	vec3f   pos(playerCam->camPos.x,playerCam->camPos.y,playerCam->camPos.z),
			dir(cos(-playerCam->camRot.x*DEG_TO_RAD)*(-sin(-playerCam->camRot.y*DEG_TO_RAD)),
				sin(-playerCam->camRot.x*DEG_TO_RAD),
				-cos(-playerCam->camRot.x*DEG_TO_RAD)*cos(-playerCam->camRot.y*DEG_TO_RAD)),
			vox(floor(pos.x), floor(pos.y), floor(pos.z)),
			step(0,0,0),
			next(0,0,0),
			tMaxc(tMax,tMax,tMax),
			tDelta(tMax,tMax,tMax);

	if (dir.x < 0) step.x = -1;
	else step.x = 1;
	if (dir.y < 0) step.y = -1;
	else step.y = 1;
	if (dir.z < 0) step.z = -1;
	else step.z = 1;

	next.x = vox.x + (step.x > 0 ? 1 : 0);
	next.y = vox.y + (step.y > 0 ? 1 : 0);
	next.z = vox.z + (step.z > 0 ? 1 : 0);

	if (dir.x != 0) {
		tDelta.x = step.x/dir.x;
		tMaxc.x = (next.x - pos.x)/dir.x;
	}
	if (dir.y != 0) {
		tDelta.y = step.y/dir.y;
		tMaxc.y = (next.y - pos.y)/dir.y;
	}
	if (dir.z != 0) {
		tDelta.z = step.z/dir.z;
		tMaxc.z = (next.z - pos.z)/dir.z;
	}

	float tCurr = 0;
	while (tCurr < tMax) {
		last = vox;
		if(tMaxc.x < tMaxc.y) {
			if(tMaxc.x < tMaxc.z) {
				tCurr = tMaxc.x;
				tMaxc.x = tMaxc.x + tDelta.x;
				vox.x = vox.x + step.x;
			}
			else {
				tCurr = tMaxc.z;
				vox.z = vox.z + step.z;
				tMaxc.z = tMaxc.z + tDelta.z;

			}
		}
		else {
			if(tMaxc.y < tMaxc.z) {
				tCurr = tMaxc.y;
				vox.y = vox.y + step.y;
				tMaxc.y = tMaxc.y + tDelta.y;
			}
			else {
				tCurr = tMaxc.z;
				vox.z = vox.z + step.z;
				tMaxc.z= tMaxc.z + tDelta.z;
			}
		}
		if(!getOutOfBounds(vox.x,vox.y,vox.z) && getCube(vox.x,vox.y,vox.z).ID != 0) {
			playerTargetsBlock = true;
			targetedBlock = vox;
			return;
		}
	}
	playerTargetsBlock = false;
}


void World::calculateLight(vec3i source, vec2i radius) {
	//BFS TO THE MAX
	std::vector<vec3i> blocksToCheck[MAXLIGHT+1];
	unsigned char ID = 0;
	unsigned char light = 0;
	for(int x = source.x-radius.x; x <= source.x+radius.x; ++x) {
		for(int y = source.y-radius.y; y <= source.y+radius.y; ++y) {
			for(int z = source.z-radius.x; z <= source.z+radius.x; ++z) {
				if (!getOutOfBounds(x,y,z)){
					std::pair<vec3i,vec3i> matrixCoords = getCoords(x,y,z);
					if((*this)(matrixCoords.first.x,matrixCoords.first.y,matrixCoords.first.z) != NULL) {
						ID = (*this)(matrixCoords.first.x,matrixCoords.first.y,matrixCoords.first.z)->cubes[matrixCoords.second.x][matrixCoords.second.y][matrixCoords.second.z].ID;
						light = (*this)(matrixCoords.first.x,matrixCoords.first.y,matrixCoords.first.z)->cubes[matrixCoords.second.x][matrixCoords.second.y][matrixCoords.second.z].light;
						switch(ID) {
							case 0: //air
								if (x == source.x-radius.x || x == source.x+radius.x
									||y == source.y-radius.y || y == source.y+radius.y
									||z == source.z-radius.x || z == source.z+radius.x){
									//if it is on border, mark it as node
									if (light > MINLIGHT) {
										blocksToCheck[light].push_back(vec3i(x,y,z));
									}
								}
								else {
									if (getSkyAccess(x,y,z)) {
										setCubeLight(x,y,z,MAXLIGHT);
										blocksToCheck[MAXLIGHT].push_back(vec3i(x,y,z));
									}
									else
										setCubeLight(x,y,z,MINLIGHT);
									(*this)(matrixCoords.first.x,matrixCoords.first.y,matrixCoords.first.z)->markedForRedraw = true;
								}
								break;
							case 4: //lightblock
								setCubeLight(x,y,z,MAXLIGHT);
								(*this)(matrixCoords.first.x,matrixCoords.first.y,matrixCoords.first.z)->markedForRedraw = true;
								blocksToCheck[MAXLIGHT].push_back(vec3i(x,y,z));
								break;
							default:
								setCubeLight(x,y,z,0);
								(*this)(matrixCoords.first.x,matrixCoords.first.y,matrixCoords.first.z)->markedForRedraw = true;
								break;
						}
					}
				}
			}
		}
	}
	for (int i = MAXLIGHT; i > MINLIGHT; --i)  {
		for(uint j = 0; j < blocksToCheck[i].size(); ++j) {
			vec3i source = blocksToCheck[i][j];
			processCubeLighting(source,vec3i(1,0,0),blocksToCheck[i-1]);
			processCubeLighting(source,vec3i(-1,0,0),blocksToCheck[i-1]);
			processCubeLighting(source,vec3i(0,1,0),blocksToCheck[i-1]);
			processCubeLighting(source,vec3i(0,-1,0),blocksToCheck[i-1]);
			processCubeLighting(source,vec3i(0,0,1),blocksToCheck[i-1]);
			processCubeLighting(source,vec3i(0,0,-1),blocksToCheck[i-1]);
		}
	}
}

void World::processCubeLighting(const vec3i& source, const vec3i& offset, std::vector<vec3i> &queue) { //BFS node processing
	vec3i subject = source+offset;
	if(!getOutOfBounds(subject.x,subject.y,subject.z)) {
		Cube sub = getCubeRaw(subject.x,subject.y,subject.z);
		if(sub.ID == 0) {
			Cube src = getCubeRaw(source.x,source.y,source.z);
			if(sub.light
			   < src.light-1) {
				queue.push_back(subject);
				setCubeLight(subject.x,subject.y,subject.z,src.light-1);
			}
		}
	}
}

void World::updateStuff(float deltaTime) { //only to be called by world.update()
	//	if (updateStuffTimer >= 0.01) { //grass thingy
	//		updateStuffTimer-= 0.01;
	//		for(int i = 0; i < 10; ++i) {
	//			int x = rand()%(CHUNKSIZE*WORLDWIDTH);
	//			int y = rand()%(CHUNKSIZE*WORLDHEIGHT);
	//			int z = rand()%(CHUNKSIZE*WORLDWIDTH);
	//			if (getCube(x,y+1,z).ID != 0 && getCube(x,y,z).ID == 3) {
	//				setCubeID(x,y,z,1);
	//			}
	//		}
	//	}
	//	updateStuffTimer += deltaTime;
}

void World::drawWireCube(const vec3f &pos) const {
	mat4f poppedMat = parentScene->getState().model;
	parentScene->getState().model.translate(pos.x-0.0025,pos.y-0.0025,pos.z-0.0025);
	parentScene->getState().model.scale(1.005,1.005,1.005);
	parentScene->getState().updateShaderUniforms(parentScene->getShader("MODEL"));
	parentScene->getShader("MODEL").use();
	glLineWidth(1.5);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor4f(0.0,0.0,0.0,0.5);
	glVertexPointer(3, GL_INT, 0, &vertexPoints[0]);
	glDrawElements(GL_LINES,24,GL_UNSIGNED_INT,&indexes[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glColor4f(1.0,1.0,1.0,1.0);
	parentScene->getState().model = poppedMat;
}

const int World::vertexPoints[8][3] = {
	{0,0,1},
	{1,0,1},
	{1,1,1},
	{0,1,1},
	{0,0,0},
	{1,0,0},
	{1,1,0},
	{0,1,0}
};

const int World::indexes[24] = {
	0,1,
	1,2,
	2,3,
	3,0,
	4,5,
	5,6,
	6,7,
	7,4,
	0,4,
	1,5,
	2,6,
	3,7,
};
