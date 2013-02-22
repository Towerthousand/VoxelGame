#include "World.hpp"
#include "Chunk.hpp"
#include "Camera.hpp"

World::World() {
	chunks = std::vector<std::vector<Chunk*> > //init chunks to 0, will never be used like this.
			 (WORLDSIZE,std::vector<Chunk*>
			  (WORLDSIZE,NULL));
	targetedBlock = sf::Vector3f(0,0,0);
	playerTargetsBlock = false;
	last = sf::Vector3f(0,0,0);
}

World::~World() {
	for (int x = 0; x < WORLDSIZE; ++x) {
		for (int z = 0; z < WORLDSIZE; ++z) {
			delete chunks[x][z];
			chunks[x][z] = NULL;
		}
	}
}

int World::getCubeAbs(int x, int y, int z) const {
	if (uint(x/CHUNKWIDTH) >= chunks.size() || uint(z/CHUNKWIDTH) >= chunks[0].size() //out of chunk grid (positive side)
		|| x%CHUNKWIDTH < 0 || z%CHUNKWIDTH < 0) //out of the chunk grid (negative side)
		return 0;
	else return chunks[x/CHUNKWIDTH][z/CHUNKWIDTH]->cubes[x%CHUNKWIDTH][y][z%CHUNKWIDTH];
}

void World::setCubeAbs(int x, int y, int z, int id) {
	chunks[x/CHUNKWIDTH][z/CHUNKWIDTH]->setCube(x%CHUNKWIDTH,y,z%CHUNKWIDTH,id);
}

void World::regenChunk(int x, int z, int seed) {
	if (chunks[x][z] != NULL)
		delete chunks[x][z];
	chunks[x][z] = new Chunk(x,z,seed,*this);
}

void World::drawWireCube(sf::Vector3f pos) const {
	glPushMatrix();
	glLineWidth(3.0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(0.0,0.0,0.0);
	glVertexPointer(3, GL_INT, 0, &vertexPoints[0]);
	glTranslatef(pos.x,pos.y,pos.z);
	glScalef(1.001,1.001,1.001);
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
	sf::Vector3f
			pos(player.posX+0.5,player.posY+0.5,player.posZ+0.5),
			dir(-m[2],-m[6],-m[10]),
			vox(floor(pos.x), floor(pos.y), floor(pos.z)),
			step,
			tMaxc,
			tDelta;

	if (dir.x < 0) step.x = -1;
	else step.x = 1;
	if (dir.y < 0) step.y = -1;
	else step.y = 1;
	if (dir.z < 0) step.z = -1;
	else step.z = 1;

	float tMax = 5;

	if (dir.x != 0) {
		tDelta.x = std::fabs(1/dir.x);
		tMaxc.x = std::fabs((vox.x - pos.x)/dir.x);
	}
	if (dir.y != 0) {
		tDelta.y = std::fabs(1/dir.y);
		tMaxc.y = std::fabs((vox.y - pos.y)/dir.y);
	};
	if (dir.z != 0) {
		tDelta.z = std::fabs(1/dir.z);
		tMaxc.z = std::fabs((vox.z - pos.z)/dir.z);
	}

	float tCurr = 0;
	while (tCurr < tMax) {
		if(tMaxc.x < tMaxc.y) {
			if(tMaxc.x < tMaxc.z) {
				tCurr = tMaxc.x;
				last = vox;
				tMaxc.x= tMaxc.x + tDelta.x;
				vox.x = vox.x + step.x;
				if(vox.x < 0 || vox.x >= WORLDSIZE*CHUNKWIDTH){
					playerTargetsBlock = false;
					return;
				}
			}
			else {
				tCurr = tMaxc.z;
				last = vox;
				vox.z = vox.z + step.z;
				if(vox.z < 0 || vox.z >= WORLDSIZE*CHUNKWIDTH) {
					playerTargetsBlock = false;
					return;
				}
				tMaxc.z= tMaxc.z + tDelta.z;

			}
		}
		else {
			if(tMaxc.y < tMaxc.z) {
				tCurr = tMaxc.y;
				last = vox;
				vox.y = vox.y + step.y;
				if(vox.y < 0 || vox.y >= CHUNKHEIGHT){
					playerTargetsBlock = false;
					return;
				}
				tMaxc.y = tMaxc.y + tDelta.y;
			}
			else {
				tCurr = tMaxc.z;
				last = vox;
				vox.z = vox.z + step.z;
				if(vox.z < 0 || vox.z >= WORLDSIZE*CHUNKWIDTH){
					playerTargetsBlock = false;
					return;
				}
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
