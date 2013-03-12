#include "World.hpp"
#include "Chunk.hpp"
#include "Camera.hpp"

World::World() {
	chunks = std::vector<std::vector<std::vector<Chunk*> > >
			 (0,std::vector<std::vector<Chunk*> >
			  (0,std::vector<Chunk*>
			   (0,NULL)));
	targetedBlock = sf::Vector3f(0,0,0);
	playerTargetsBlock = false;
	last = sf::Vector3f(0,0,0);
	chunksDrawn = 0;
	updateStuffTimer = 0.0;
}

World::~World() {
	for (int x = 0; x < WORLDWIDTH; ++x) {
		for (int y = 0; y < WORLDHEIGHT; ++y) {
			for (int z = 0; z < WORLDWIDTH; ++z) {
				delete chunks[x][y][z];
				chunks[x][y][z] = NULL;
			}
		}
	}
}


void World::loadDirbaio(const std::string filePath) {
	std::ifstream file(filePath.c_str());
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
	for (int x = 0; x < WORLDWIDTH; ++x) {
		for (int y = 0; y < WORLDHEIGHT; ++y) {
			for (int z = 0; z < WORLDWIDTH; ++z) {
				regenChunk(x,y,z,0);
			}
		}
	}
	outLog(" - Loading chunk data...");
	for(int y = 0; y < sizeY; ++y) {
		for(int x = 0; x < sizeX; ++x) {
			for(int z = 0; z < sizeZ; ++z) {
				setCubeAbs(x,y,z,Cube(file.get(),0));
			}
		}
	}
	outLog(" - Calculating sky levels...");
	skyValues = std::vector<std::vector<int> >(CHUNKSIZE*WORLDWIDTH,
											   std::vector<int>(CHUNKSIZE*WORLDWIDTH,-1));
	for(int x = 0; x < CHUNKSIZE*WORLDWIDTH; ++x) {
		for(int z = 0; z < CHUNKSIZE*WORLDWIDTH; ++z) {
			skyValues[x][z] = getSkylightLevel(x,z);
		}
	}

	outLog(" - Lighting chunks...");
	for (int x = 0; x < WORLDWIDTH; ++x) {
		for (int y = 0; y < WORLDHEIGHT; ++y) {
			for (int z = 0; z < WORLDWIDTH; ++z) {
				initChunkLight(x,y,z);
			}
		}
	}

}

void World::regenChunk(int x, int y, int z, int seed) {
	if (chunks[x][y][z] != NULL)
		delete chunks[x][y][z];
	chunks[x][y][z] = new Chunk(x,y,z,*this);
}

void World::initChunkLight(int x,int y, int z) { //should only be called if upper chunks are loaded. Coords in chunk system.
	calculateLight(sf::Vector3i(x*CHUNKSIZE+(CHUNKSIZE/2),y*CHUNKSIZE+(CHUNKSIZE/2),z*CHUNKSIZE+(CHUNKSIZE/2)),sf::Vector2i(CHUNKSIZE,CHUNKSIZE),false);
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

Cube World::getCubeAbs(int x, int y, int z) const {
	if (getOutOfBounds(x,y,z))
		return Cube(0,MINLIGHT);
	return chunks[x/CHUNKSIZE][y/CHUNKSIZE][z/CHUNKSIZE]->cubes[x%CHUNKSIZE][y%CHUNKSIZE][z%CHUNKSIZE];
}

void World::setCubeAbs(int x, int y, int z, const Cube &c) {
	if (getOutOfBounds(x,y,z))
		return;
	chunks[x/CHUNKSIZE][y/CHUNKSIZE][z/CHUNKSIZE]->cubes[x%CHUNKSIZE][y%CHUNKSIZE][z%CHUNKSIZE] = c;
	chunks[x/CHUNKSIZE][y/CHUNKSIZE][z/CHUNKSIZE]->markedForRedraw = true;
}

int World::getSkylightLevel(int x, int z) { //X and Z in cube coords
	for(int y = CHUNKSIZE*WORLDHEIGHT-1; y >= 0; --y) {
		if(getCubeAbs(x,y,z).ID != 0) {
			return y;
		}
	}
	return -1;
}

bool World::getSkyAccess(int x, int y, int z) {
	if (y <= skyValues[x][z])
		return false;
	return true;
}

void World::draw() const {
	for (int x = 0; x < WORLDWIDTH; ++x)  {
		for (int y = 0; y < WORLDHEIGHT; ++y) {
			for (int z = 0; z < WORLDWIDTH; ++z) {
				if(!chunks[x][y][z]->outOfView) {
					chunks[x][y][z]->draw();
				}
			}
		}
	}
	if (playerTargetsBlock)
		drawWireCube(targetedBlock);
}

void World::update(float deltaTime, const Camera& camera) {
	chunksDrawn = 0;
	updateGrass(deltaTime);
	for (int x = 0; x < WORLDWIDTH; ++x)  {
		for (int y = 0; y < WORLDHEIGHT; ++y) {
			for (int z = 0; z < WORLDWIDTH; ++z) {
				if (chunks[x][y][z]->checkCulling(camera)){
					chunks[x][y][z]->outOfView = true;
				}
				else {
					++chunksDrawn;
					chunks[x][y][z]->outOfView = false;
				}
				chunks[x][y][z]->update(deltaTime);
			}
		}
	}
}

//Based on: Fast Voxel Traversal Algorithm for Ray Tracing
//By: John Amanatides et al.
//Implemented by Jordi "BuD" Santiago Provencio
void World::traceView(const Camera& player, float tMax) {
	if (!getOutOfBounds(floor(player.pos.x),floor(player.pos.y),floor(player.pos.z)) &&
		getCubeAbs(floor(player.pos.x),floor(player.pos.y),floor(player.pos.z)).ID != 0) {
		playerTargetsBlock = true;
		targetedBlock = sf::Vector3f(floor(player.pos.x),floor(player.pos.y),floor(player.pos.z));
		return;
	}

	sf::Vector3f
			pos(player.pos.x,player.pos.y,player.pos.z),
			dir(cos(-player.rot.x*DEG_TO_RAD)*(-sin(-player.rot.y*DEG_TO_RAD)),
				sin(-player.rot.x*DEG_TO_RAD),
				-cos(-player.rot.x*DEG_TO_RAD)*cos(-player.rot.y*DEG_TO_RAD)),
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
		if(tMaxc.x < tMaxc.y) {
			if(tMaxc.x < tMaxc.z) {
				tCurr = tMaxc.x;
				last = vox;
				tMaxc.x= tMaxc.x + tDelta.x;
				vox.x = vox.x + step.x;
			}
			else {
				tCurr = tMaxc.z;
				last = vox;
				vox.z = vox.z + step.z;
				tMaxc.z = tMaxc.z + tDelta.z;

			}
		}
		else {
			if(tMaxc.y < tMaxc.z) {
				tCurr = tMaxc.y;
				last = vox;
				vox.y = vox.y + step.y;
				tMaxc.y = tMaxc.y + tDelta.y;
			}
			else {
				tCurr = tMaxc.z;
				last = vox;
				vox.z = vox.z + step.z;
				tMaxc.z= tMaxc.z + tDelta.z;
			}
		}
		if(!getOutOfBounds(vox.x,vox.y,vox.z) && getCubeAbs(vox.x,vox.y,vox.z).ID != 0) {
			playerTargetsBlock = true;
			targetedBlock = vox;
			return;
		}
	}
	playerTargetsBlock = false;
}


void World::calculateLight(sf::Vector3i source, sf::Vector2i radius, bool changedBlock) {
	//BFS TO THE MAX
	if (source.y > skyValues[source.x][source.z] && changedBlock) { // calculate "shadow" of the new block
		int previousSkyValue = skyValues[source.x][source.z];
		skyValues[source.x][source.z] = getSkylightLevel(source.x,source.z);
		calculateLight(sf::Vector3i(source.x,
									((source.y - previousSkyValue	)/2) + previousSkyValue,
									source.z),
					   sf::Vector2i(UPDATERADIUS,(source.y - previousSkyValue)/2 + UPDATERADIUS),false);
		return;
	}
	else if (source.y == skyValues[source.x][source.z] && changedBlock) { //propagate skylight downwards
		skyValues[source.x][source.z] = getSkylightLevel(source.x,source.z);
		calculateLight(sf::Vector3i(source.x,
									((source.y - skyValues[source.x][source.z])/2) + skyValues[source.x][source.z],
									source.z),
					   sf::Vector2i(UPDATERADIUS,(source.y - skyValues[source.x][source.z])/2 + UPDATERADIUS),false);
		return;
	}
	std::queue<sf::Vector3i> blocksToCheck;
	for(int x = source.x-radius.x; x <= source.x+radius.x; ++x) {
		for(int y = source.y-radius.y; y <= source.y+radius.y; ++y) {
			for(int z = source.z-radius.x; z <= source.z+radius.x; ++z) {
				if (!getOutOfBounds(x,y,z)){
					switch(getCubeAbs(x,y,z).ID) {
						case 0: //air
							if (x == source.x-radius.x || x == source.x+radius.x
								||y == source.y-radius.y || y == source.y+radius.y
								||z == source.z-radius.x || z == source.z+radius.x){
								//if it is on border, mark it as node
								if (getCubeAbs(x,y,z).light > MINLIGHT) {
									blocksToCheck.push(sf::Vector3i(x,y,z));
								}
							}
							else {
								if (getSkyAccess(x,y,z)) {
									setCubeAbs(x,y,z,Cube(0,MAXLIGHT));
									blocksToCheck.push(sf::Vector3i(x,y,z));
								}
								else setCubeAbs(x,y,z,Cube(0,MINLIGHT));
							}
							break;
						case 4: //lightblock
							setCubeAbs(x,y,z,Cube(4,MAXLIGHT+5));
							blocksToCheck.push(sf::Vector3i(x,y,z));
							break;
						default:
							setCubeAbs(x,y,z,Cube(getCubeAbs(x,y,z).ID,0));
							break;
					}
				}
			}
		}
	}
	while(!blocksToCheck.empty()) {
		sf::Vector3i source = blocksToCheck.front();
		if(getCubeAbs(source.x,source.y,source.z).light*LIGHTFACTOR <= MINLIGHT) {
			setCubeAbs(source.x,source.y,source.z,Cube(0,MINLIGHT));
			blocksToCheck.pop();
		}
		else {
			processCubeLighting(source,sf::Vector3i(1,0,0),blocksToCheck);
			processCubeLighting(source,sf::Vector3i(-1,0,0),blocksToCheck);
			processCubeLighting(source,sf::Vector3i(0,1,0),blocksToCheck);
			processCubeLighting(source,sf::Vector3i(0,-1,0),blocksToCheck);
			processCubeLighting(source,sf::Vector3i(0,0,1),blocksToCheck);
			processCubeLighting(source,sf::Vector3i(0,0,-1),blocksToCheck);
			blocksToCheck.pop();
		}
	}
}

void World::processCubeLighting(const sf::Vector3i& source, const sf::Vector3i& offset, std::queue<sf::Vector3i> &queue) { //BFS node processing
	sf::Vector3i subject = source+offset;
	if(!getOutOfBounds(subject.x,subject.y,subject.z) && getCubeAbs(subject.x,subject.y,subject.z).ID == 0) {
		if(getCubeAbs(subject.x,subject.y,subject.z).light < getCubeAbs(source.x,source.y,source.z).light*LIGHTFACTOR) {
			queue.push(subject);
			setCubeAbs(subject.x,subject.y,subject.z,
					   Cube(0,getCubeAbs(source.x,source.y,source.z).light*LIGHTFACTOR));
		}
	}
}

void World::updateGrass(float deltaTime) { //only to be called by world.update()
	if (updateStuffTimer >= 0.01) {
		updateStuffTimer-= 0.01;
		for(int i = 0; i < 10; ++i) {
			int x = rand()%(CHUNKSIZE*WORLDWIDTH);
			int y = rand()%(CHUNKSIZE*WORLDHEIGHT);
			int z = rand()%(CHUNKSIZE*WORLDWIDTH);
			if (getCubeAbs(x,y+1,z).ID != 0 && getCubeAbs(x,y,z).ID == 3) {
				setCubeAbs(x,y,z,Cube(1,0));
			}
		}
	}
	updateStuffTimer += deltaTime;
}

void World::drawWireCube(const sf::Vector3f &pos) const {
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
