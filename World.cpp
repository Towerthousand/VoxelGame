#include "World.hpp"
#include "Chunk.hpp"
#include "Camera.hpp"

World::World() {
	chunks = std::vector<std::vector<Chunk*> > //init chunks to 0, will never be used like this.
			 (WORLDSIZE,std::vector<Chunk*>
			  (WORLDSIZE,NULL));
	targetedBlock = sf::Vector3<double>(0,0,0);
	playerTargetsBlock = false;
	last = sf::Vector3<double>(0,0,0);
}

World::~World() {
	for (int x = 0; x < WORLDSIZE; ++x) {
		for (int z = 0; z < WORLDSIZE; ++z) {
			delete chunks[x][z];
			chunks[x][z] = NULL;
		}
	}
}

int World::getCubeAbs(float x, float y, float z) const {
	x = floor(x);
	y = floor(y);
	z = floor(z);
	if (   x/CHUNKWIDTH >= WORLDSIZE || x/CHUNKWIDTH < 0
		   || z/CHUNKWIDTH >= WORLDSIZE || z/CHUNKWIDTH < 0
		   || y >= CHUNKHEIGHT
		   || int(x)%CHUNKWIDTH < 0
		   || int(z)%CHUNKWIDTH < 0
		   || y < 0
		   || z < 0
		   || x < 0)
		return 0;
	else return chunks[x/CHUNKWIDTH][z/CHUNKWIDTH]->cubes[int(x)%CHUNKWIDTH][y][int(z)%CHUNKWIDTH];
}

void World::setCubeAbs(int x, int y, int z, int id) {
	if (   x/CHUNKWIDTH >= WORLDSIZE || x/CHUNKWIDTH < 0
		   || z/CHUNKWIDTH >= WORLDSIZE || z/CHUNKWIDTH < 0
		   || y >= CHUNKHEIGHT
		   || x%CHUNKWIDTH < 0
		   || z%CHUNKWIDTH < 0
		   || y < 0 )
		return;
	chunks[x/CHUNKWIDTH][z/CHUNKWIDTH]->setCube(x%CHUNKWIDTH,y,z%CHUNKWIDTH,id);
	if(x%CHUNKWIDTH == 0 && x/CHUNKWIDTH > 0)
		chunks[x/CHUNKWIDTH - 1][z/CHUNKWIDTH]->markedForRedraw = true;
	if(x%CHUNKWIDTH == CHUNKWIDTH-1 && x/CHUNKWIDTH < WORLDSIZE-1)
		chunks[x/CHUNKWIDTH + 1][z/CHUNKWIDTH]->markedForRedraw = true;
	if(z%CHUNKWIDTH == 0 && z/CHUNKWIDTH > 0)
		chunks[x/CHUNKWIDTH][z/CHUNKWIDTH - 1]->markedForRedraw = true;
	if(z%CHUNKWIDTH == CHUNKWIDTH-1 && z/CHUNKWIDTH < WORLDSIZE-1)
		chunks[x/CHUNKWIDTH][z/CHUNKWIDTH + 1]->markedForRedraw = true;
}

void World::regenChunk(int x, int z, int seed) {
	if (chunks[x][z] != NULL)
		delete chunks[x][z];
	chunks[x][z] = new Chunk(x,z,seed,*this);
}

void World::drawWireCube(sf::Vector3<double> pos) const {
	glPushMatrix();
	glLineWidth(3.0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(0.0,0.0,0.0);
	glVertexPointer(3, GL_INT, 0, &vertexPoints[0]);
	glTranslatef(pos.x,pos.y,pos.z);
	glDrawElements(GL_LINES,24,GL_UNSIGNED_INT,&indexes[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glColor3f(1.0,1.0,1.0);
	glPopMatrix();
}

void World::draw() const {
	for (int x = 0; x < WORLDSIZE; ++x)  {
		for (int z = 0; z < WORLDSIZE; ++z) {
			chunks[x][z]->draw();
		}
	}

	if (playerTargetsBlock)
		drawWireCube(targetedBlock);
}

void World::update(float deltaTime) {
	for (int x = 0; x < WORLDSIZE; ++x)  {
		for (int z = 0; z < WORLDSIZE; ++z) {
			chunks[x][z]->update(deltaTime);
		}
	}
}

//Based on: Fast Voxel Traversal Algorithm for Ray Tracing
//By: John Amanatides et al.
void World::traceView(const Camera& player) {
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	sf::Vector3<double>
			pos(player.pos.x,player.pos.y,player.pos.z),
			dir(cos(-player.rot.x*DEG_TO_RAD)*(-sin(-player.rot.y*DEG_TO_RAD)),
				sin(-player.rot.x*DEG_TO_RAD),
				-cos(-player.rot.x*DEG_TO_RAD)*cos(-player.rot.y*DEG_TO_RAD)),
			//dir(-m[2],-m[6],-m[10]),
			//dir(1,-1,0),
			vox(floor(pos.x), floor(pos.y), floor(pos.z)),
			step(0,0,0),
			tMaxc,
			tDelta;

	//normalize(dir);

	if (dir.x < 0) step.x = -1;
	else step.x = 1;
	if (dir.y < 0) step.y = -1;
	else step.y = 1;
	if (dir.z < 0) step.z = -1;
	else step.z = 1;

	float tMax = 50; //Range of view

	if (dir.x != 0) {
		tDelta.x = std::fabs(1.0f/dir.x);
		tMaxc.x = (pos.x - vox.x)/dir.x;
	}
	else tMaxc.x = 10000.0;
	if (dir.y != 0) {
		tDelta.y = std::fabs(1.0f/dir.y);
		tMaxc.y = (pos.y - vox.y)/dir.y;
	}
	else tMaxc.y = 10000.0;
	if (dir.z != 0) {
		tDelta.z = std::fabs(1.0f/dir.z);
		tMaxc.z = (pos.z - vox.z)/dir.z;
	}
	else tMaxc.z = 10000.0;

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
		if(getCubeAbs(vox.x,vox.y,vox.z) != 0) {
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
