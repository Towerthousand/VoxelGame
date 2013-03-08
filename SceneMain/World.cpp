#include "World.hpp"
#include "Chunk.hpp"
#include "Camera.hpp"

World::World() {
	chunks = std::vector<std::vector<std::vector<Chunk*> > >
			 (WORLDWIDTH,std::vector<std::vector<Chunk*> >
			  (WORLDHEIGHT,std::vector<Chunk*>
			   (WORLDWIDTH,NULL)));
	targetedBlock = sf::Vector3f(0,0,0);
	playerTargetsBlock = false;
	last = sf::Vector3f(0,0,0);
	chunksDrawn = 0;
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

bool World::outOfBounds(int x, int y, int z){
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
    return chunks[x/CHUNKSIZE][y/CHUNKSIZE][z/CHUNKSIZE]->cubes[x%CHUNKSIZE][y%CHUNKSIZE][z%CHUNKSIZE];
}

void World::setCubeAbs(int x, int y, int z, Cube c) {
    chunks[x/CHUNKSIZE][y/CHUNKSIZE][z/CHUNKSIZE]->cubes[x%CHUNKSIZE][y%CHUNKSIZE][z%CHUNKSIZE] = c;
    chunks[x/CHUNKSIZE][y/CHUNKSIZE][z/CHUNKSIZE]->markedForRedraw = true;
}

void World::regenChunk(int x, int y, int z, int seed) {
	if (chunks[x][y][z] != NULL)
		delete chunks[x][y][z];
	chunks[x][y][z] = new Chunk(x,y,z,*this);
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
	if (!outOfBounds(floor(player.pos.x),floor(player.pos.y),floor(player.pos.z)) &&
		getCubeAbs(floor(player.pos.x),floor(player.pos.y),floor(player.pos.z)).id != 0) {
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
		if(!outOfBounds(vox.x,vox.y,vox.z) && getCubeAbs(vox.x,vox.y,vox.z).id != 0) {
			playerTargetsBlock = true;
			targetedBlock = vox;
			return;
		}
	}
	playerTargetsBlock = false;
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

const Cube World::empty(0,10);
