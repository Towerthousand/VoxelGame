#include "World.hpp"
#include "Chunk.hpp"
#include "../entities/Player.hpp"
#include "../SceneMain.hpp"

World::World(SceneMain* parentScene, Player* player) :
	playerTargetsBlock(false), targetedBlock(0,0,0),
	last(0,0,0), parentScene(parentScene), player(player),
	chunks(0,std::vector<std::vector<Chunk*> >
		   (0,std::vector<Chunk*>
			(0,NULL))),
	updateStuffTimer(0.0){
}

World::~World() {
	for (int x = 0; x < WORLDWIDTH; ++x)
		for (int y = 0; y < WORLDHEIGHT; ++y)
			for (int z = 0; z < WORLDWIDTH; ++z) {
				delete chunks[x][y][z];
				chunks[x][y][z] = NULL;
			}
}

bool World::loadDirbaio(const std::string &filePath) {
	sf::Clock clock;
	clock.restart();
	std::ifstream file(filePath.c_str());
	if(!file) {
		outLog("#ERROR Could not load dirbaio \"" + filePath + "\"");
		return false;
	}
	int sizeX = int(file.get()) << 24 | int(file.get()) << 16 | int(file.get()) << 8 | int(file.get());
	int sizeY = int(file.get()) << 24 | int(file.get()) << 16 | int(file.get()) << 8 | int(file.get());
	int sizeZ = int(file.get()) << 24 | int(file.get()) << 16 | int(file.get()) << 8 | int(file.get());
	WORLDWIDTH = sizeX/CHUNKSIZE;
	WORLDHEIGHT = sizeY/CHUNKSIZE;
	chunks.resize(WORLDWIDTH);
	for(int i = 0; i < WORLDWIDTH; ++i)
		chunks[i].resize(WORLDHEIGHT);
	for(int i = 0; i < WORLDWIDTH; ++i)
		for(int j = 0; j < WORLDHEIGHT; ++j)
			chunks[i][j].resize(WORLDWIDTH);

	outLog(" - Creating chunks...");
	for (int x = 0; x < WORLDWIDTH; ++x)
		for (int y = 0; y < WORLDHEIGHT; ++y)
			for (int z = 0; z < WORLDWIDTH; ++z)
				chunks[x][y][z] = new Chunk(x,y,z,this);
	outLog(" - Loading chunk data...");
	for(int y = 0; y < sizeY; ++y)
		for(int x = 0; x < sizeX; ++x)
			for(int z = 0; z < sizeZ; ++z)
				chunks[x/CHUNKSIZE][y/CHUNKSIZE][z/CHUNKSIZE]->cubes[x%CHUNKSIZE][y%CHUNKSIZE][z%CHUNKSIZE] = Cube(file.get(),0);
	file.close();
	outLog(" - Calculating sky levels...");
	skyValues = std::vector<std::vector<int> >(CHUNKSIZE*WORLDWIDTH,
											   std::vector<int>(CHUNKSIZE*WORLDWIDTH,-1));
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
	if (x/CHUNKSIZE >= WORLDWIDTH || x/CHUNKSIZE < 0
		|| y/CHUNKSIZE >= WORLDHEIGHT || y/CHUNKSIZE < 0
		|| z/CHUNKSIZE >= WORLDWIDTH || z/CHUNKSIZE < 0
		|| x%CHUNKSIZE < 0
		|| z%CHUNKSIZE < 0
		|| y%CHUNKSIZE < 0)
		return true;
	return false;
}

Cube World::getCube(int x, int y, int z) const {
	if (getOutOfBounds(x,y,z))
		return Cube(0,MAXLIGHT);
	return getCubeRaw(x,y,z);
}

Cube World::getCubeRaw(int x, int y, int z) const {
	return chunks[x/CHUNKSIZE][y/CHUNKSIZE][z/CHUNKSIZE]->cubes[x%CHUNKSIZE][y%CHUNKSIZE][z%CHUNKSIZE];
}

int World::getSkylightLevel(int x, int z) const { //X and Z in cube coords
	for(int y = CHUNKSIZE*WORLDHEIGHT-1; y >= 0; --y)
		if(getCube(x,y,z).ID != 0)
			return y;
	return -1;
}

bool World::getSkyAccess(int x, int y, int z) const {
	if (y <= skyValues[x][z])
		return false;
	return true;
}

void World::setCubeID(int x, int y, int z, unsigned char ID) { //set the id, calculate light (taking into account sky level)
	if (getOutOfBounds(x,y,z))
		return;
	chunks[x/CHUNKSIZE][y/CHUNKSIZE][z/CHUNKSIZE]->cubes[x%CHUNKSIZE][y%CHUNKSIZE][z%CHUNKSIZE].ID = ID;
	if (y > skyValues[x][z]) { // calculate "shadow" of the new block
		int previousSkyValue = skyValues[x][z];
		skyValues[x][z] = getSkylightLevel(x,z);
		calculateLight(vec3i(x,
							 ((y - previousSkyValue)/2) + previousSkyValue - UPDATERADIUS/2,
							 z),
					   vec2i(UPDATERADIUS,std::max(UPDATERADIUS,(y - previousSkyValue)/2 + UPDATERADIUS/2 + 4)));
	}
	else if (y == skyValues[x][z]) { //propagate skylight downwards
		skyValues[x][z] = getSkylightLevel(x,z);
		calculateLight(vec3i(x,
							 ((y - skyValues[x][z])/2) + skyValues[x][z]  - UPDATERADIUS/2,
							 z),
					   vec2i(UPDATERADIUS,std::max(UPDATERADIUS,(y - skyValues[x][z])/2 + UPDATERADIUS/2 + 4)));
	}
	else { //just calculate the surrounding blocks, since wer'e not changing sky level
		calculateLight(vec3i(x,y,z),vec2i(UPDATERADIUS,UPDATERADIUS));
	}
}

void World::setCubeLight(int x, int y, int z, unsigned char light) { //set the light, mark for redraw
	if (getOutOfBounds(x,y,z))
		return;
	chunks[x/CHUNKSIZE][y/CHUNKSIZE][z/CHUNKSIZE]->cubes[x%CHUNKSIZE][y%CHUNKSIZE][z%CHUNKSIZE].light = light;
	chunks[x/CHUNKSIZE][y/CHUNKSIZE][z/CHUNKSIZE]->markedForRedraw = true;
}

void World::draw() const {
	parentScene->chunksDrawn = 0;
	//empty culing
	for (int x = 0; x < WORLDWIDTH; ++x)
		for (int y = 0; y < WORLDHEIGHT; ++y)
			for (int z = 0; z < WORLDWIDTH; ++z) {
				if (chunks[x][y][z]->vertexCount == 0)
					chunks[x][y][z]->outOfView = true;
				else
					chunks[x][y][z]->outOfView = !player->insideFrustum(vec3f(x*CHUNKSIZE+CHUNKSIZE/2,
																			  y*CHUNKSIZE+CHUNKSIZE/2,
																			  z*CHUNKSIZE+CHUNKSIZE/2)
																		,sqrt(3*(8*8)));
			}

	//do occlusion culling here!
	std::priority_queue<std::pair<float,Chunk*> > queryList; //chunks to be queried, ordered by distance

	//sort by distance
	float dist = 0;
	for(int x = 0; x < WORLDWIDTH; ++x)
		for(int y = 0; y < WORLDHEIGHT; ++y)
			for(int z = 0; z < WORLDWIDTH; ++z)
				if(!chunks[x][y][z]->outOfView) {
					dist = (vec3f(x*CHUNKSIZE,y*CHUNKSIZE,z*CHUNKSIZE) + vec3f(CHUNKSIZE/2,CHUNKSIZE/2,CHUNKSIZE/2) - parentScene->player->pos).module();
					queryList.push(std::pair<float,Chunk*>(-dist,chunks[x][y][z]));
				}

	int layers = 5;
	int chunksPerLayer = queryList.size()/layers + int(queryList.size()%layers > 0); //chunks per pass
	std::vector<GLuint> queries(chunksPerLayer,0);

	//first layer is always drawn
	for(int i = 0; i < chunksPerLayer && queryList.size() > 0; i++) {
		std::pair<float,Chunk*> c = queryList.top();
		queryList.pop();
		c.second->draw();
		++parentScene->chunksDrawn;
	}
	//Query other layers
	for(int currLayer = 1; currLayer < layers && queryList.size() > 0; ++currLayer) {
		std::vector<Chunk*> chunkPointers(chunksPerLayer,NULL);

		//disable rendering state
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);
		glDisable(GL_LIGHTING);

		//generate and send the queries
		int queriesSent = 0;
		glGenOcclusionQueriesNV(chunksPerLayer, &queries[0]);
		for (int i = 0; i < chunksPerLayer && queryList.size() > 0; ++i) {
			Chunk* currChunk = queryList.top().second;
			chunkPointers[i] = currChunk;
			queryList.pop();

			glBeginOcclusionQueryNV(queries[i]);
			currChunk->drawBoundingBox();
			glEndOcclusionQueryNV();
			++queriesSent;
		}

		//enable rendering state
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);
		glDisable(GL_LIGHTING);

		//collect query results
		for (int i = 0; i < queriesSent; ++i) {
			//if we have pending query, get result
			GLuint pixelCount;
			glGetOcclusionQueryuivNV(queries[i], GL_PIXEL_COUNT_NV, &pixelCount);
			//if seen, draw it
			if (pixelCount != 0) {
				chunkPointers[i]->draw();
				++parentScene->chunksDrawn;
			}
		}
	}
	if (playerTargetsBlock)
		drawWireCube(targetedBlock);
}

void World::update(float deltaTime) {
	//updateStuff(deltaTime);
	traceView(player,10);
	for (int x = 0; x < WORLDWIDTH; ++x)
		for (int y = 0; y < WORLDHEIGHT; ++y)
			for (int z = 0; z < WORLDWIDTH; ++z)
				if (chunks[x][y][z]->markedForRedraw == true)
					chunks[x][y][z]->update(deltaTime);
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
	unsigned char* ID = 0;
	unsigned char* light = 0;
	for(int x = source.x-radius.x; x <= source.x+radius.x; ++x) {
		for(int y = source.y-radius.y; y <= source.y+radius.y; ++y) {
			for(int z = source.z-radius.x; z <= source.z+radius.x; ++z) {
				if (!getOutOfBounds(x,y,z)){
					ID = &chunks[x/CHUNKSIZE][y/CHUNKSIZE][z/CHUNKSIZE]->cubes[x%CHUNKSIZE][y%CHUNKSIZE][z%CHUNKSIZE].ID;
					light = &chunks[x/CHUNKSIZE][y/CHUNKSIZE][z/CHUNKSIZE]->cubes[x%CHUNKSIZE][y%CHUNKSIZE][z%CHUNKSIZE].light;
					switch(*ID) {
						case 0: //air
							if (x == source.x-radius.x || x == source.x+radius.x
								||y == source.y-radius.y || y == source.y+radius.y
								||z == source.z-radius.x || z == source.z+radius.x){
								//if it is on border, mark it as node
								if (*light > MINLIGHT) {
									blocksToCheck[*light].push_back(vec3i(x,y,z));
								}
							}
							else {
								if (getSkyAccess(x,y,z)) {
									*light = MAXLIGHT;
									blocksToCheck[MAXLIGHT].push_back(vec3i(x,y,z));
								}
								else
									*light = MINLIGHT;
								chunks[x/CHUNKSIZE][y/CHUNKSIZE][z/CHUNKSIZE]->markedForRedraw = true;
							}
							break;
						case 4: //lightblock
							*light = MAXLIGHT;
							chunks[x/CHUNKSIZE][y/CHUNKSIZE][z/CHUNKSIZE]->markedForRedraw = true;
							blocksToCheck[MAXLIGHT].push_back(vec3i(x,y,z));
							break;
						default:
							*light = 0;
							chunks[x/CHUNKSIZE][y/CHUNKSIZE][z/CHUNKSIZE]->markedForRedraw = true;
							break;
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
	if (updateStuffTimer >= 0.01) { //grass thingy
		updateStuffTimer-= 0.01;
		for(int i = 0; i < 10; ++i) {
			int x = rand()%(CHUNKSIZE*WORLDWIDTH);
			int y = rand()%(CHUNKSIZE*WORLDHEIGHT);
			int z = rand()%(CHUNKSIZE*WORLDWIDTH);
			if (getCube(x,y+1,z).ID != 0 && getCube(x,y,z).ID == 3) {
				setCubeID(x,y,z,1);
			}
		}
	}
	updateStuffTimer += deltaTime;
}

void World::drawWireCube(const vec3f &pos) const {
	glPushMatrix();
	glLineWidth(1.5);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor4f(0.0,0.0,0.0,0.5);
	glVertexPointer(3, GL_INT, 0, &vertexPoints[0]);
	glTranslatef(pos.x-0.0025,pos.y-0.0025,pos.z-0.0025);
	glScalef(1.005,1.005,1.005);
	glDrawElements(GL_LINES,24,GL_UNSIGNED_INT,&indexes[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glColor4f(1.0,1.0,1.0,1.0);
	glPopMatrix();
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
