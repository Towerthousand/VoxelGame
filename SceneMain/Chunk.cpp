#include "Chunk.hpp"
#include "World.hpp"

Chunk::Chunk(int x, int y, int z, World &world) : XPOS(x), ZPOS(z), YPOS(y), parentWorld(world){
	cubes = std::vector<std::vector<std::vector<Cube> > >
			(CHUNKSIZE,std::vector<std::vector<Cube> >
			 (CHUNKSIZE,std::vector<Cube>
              (CHUNKSIZE,Cube(0,1))));
	VBOID = XPOS*WORLDWIDTH+ZPOS*WORLDWIDTH+YPOS*WORLDHEIGHT+1;
	outOfView = false;
	markedForRedraw = false;
	grassTimer = 0.0;
	populate();
}

Chunk::~Chunk() {
}

void Chunk::populate() {
	for(int x = CHUNKSIZE*(0.5/4.0); x < CHUNKSIZE*(3.5/4.0); ++x) {
		for(int z = CHUNKSIZE*(0.5/4.0); z < CHUNKSIZE*(3.5/4.0); ++z) {
			for(int y = CHUNKSIZE*(0.5/4.0); y < CHUNKSIZE*(3.5/4.0); ++y) {
                cubes[x][y][z].id = rand()%3+1;
			}
		}
	}
	markedForRedraw = true;
}

bool Chunk::outOfBounds(int x, int y, int z) {
	return parentWorld.outOfBounds(x+(XPOS*CHUNKSIZE),y+(YPOS*CHUNKSIZE),z+(ZPOS*CHUNKSIZE));
}

Cube Chunk::getCube(int x, int y, int z) {
	if (x >= CHUNKSIZE || y >= CHUNKSIZE || z >= CHUNKSIZE || x < 0 || z < 0 || y < 0)//outside of this chunk
		return parentWorld.getCubeAbs(x+(XPOS*CHUNKSIZE),y+(YPOS*CHUNKSIZE),z+(ZPOS*CHUNKSIZE));
	return cubes[x][y][z]; //inside current chunk
}

void Chunk::update(float deltaTime) {
	//updateGrass(deltaTime);
	if (markedForRedraw) { //empty arrays and re-do them
		markedForRedraw = false;
		DBG_UPDATES++;
		renderData.resize(0);
		int cubeID;
		for(int z = 0; z < CHUNKSIZE; ++z) {
			for(int y = 0; y < CHUNKSIZE; ++y) {
				for(int x = 0; x < CHUNKSIZE; ++x) {
					cubeID = getCube(x,y,z).id;
					if (cubeID != 0) { // only draw if it's not air
						pushCubeToArray(x,y,z,cubeID);
					}
				}
			}
		}
		makeVbo();
	}
}


void Chunk::draw() const {
	glPushMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(GLvoid*)(3*sizeof(float)));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid*)(6*sizeof(float)));
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), (GLvoid*)(8*sizeof(float)));

	glDrawArrays(GL_QUADS, 0, renderData.size());

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glPopMatrix();
}

bool Chunk::checkCulling(const Camera& cam) {
	sf::Vector2f point(XPOS*CHUNKSIZE+CHUNKSIZE/2, ZPOS*CHUNKSIZE+CHUNKSIZE/2);
	sf::Vector2f dir(-sin(-cam.rot.y*DEG_TO_RAD), -cos(-cam.rot.y*DEG_TO_RAD));
	sf::Vector2f pos(cam.pos.x,cam.pos.z);
	float distance = (dir.x*point.x + dir.y*point.y - dir.x*pos.x - dir.y*pos.y);
	return distance < -CHUNKSIZE;
}

void Chunk::pushCubeToArray(int x,int y, int z,int cubeID) {
	int absX = x+CHUNKSIZE*XPOS;
	int absY = y+CHUNKSIZE*YPOS;
	int absZ = z+CHUNKSIZE*ZPOS;
	int texY, texX;
	float lind;
	//STRUCTURE PER VERTEX: Vx,Vy,Vz,
	//						Nx,Ny,Nz,
	//						Tx,Ty,
	//						Cr,Cg,Cb,Ca
	if(!outOfBounds(x,y,z+1) && getCube(x,y,z+1).id == 0) { // front face
		lind = getCube(x,y,z+1).light/LIGHTMAX; //light index
		texX = (textureIndexes[cubeID][0] % 32)*16; // 4 = number of textures/row, 16 = width
		texY = (textureIndexes[cubeID][0] / 32)*16; // 4 = number of textures/row, 16 = height
		renderData.push_back(Vertex(absX    , absY+1.0, absZ+1.0, 0,0,1 , texX     ,texY     , lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ+1.0, 0,0,1 , texX	   ,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, absY    , absZ+1.0, 0,0,1 , texX+16.0,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ+1.0, 0,0,1 , texX+16.0,texY     , lind,lind,lind,1.0));
	}
	if(!outOfBounds(x,y,z-1) && getCube(x,y,z-1).id == 0) { // back face
		lind = getCube(x,y,z-1).light/LIGHTMAX;
		texX = (textureIndexes[cubeID][1] % 32)*16;
		texY = (textureIndexes[cubeID][1] / 32)*16;
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ    , 0,0,-1, texX     ,texY     , lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, absY    , absZ    , 0,0,-1, texX	   ,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ    , 0,0,-1, texX+16.0,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , absY+1.0, absZ    , 0,0,-1, texX+16.0,texY     , lind,lind,lind,1.0));
	}
	if(!outOfBounds(x+1,y,z) && getCube(x+1,y,z).id == 0) { // left face
		lind = getCube(x+1,y,z).light/LIGHTMAX;
		texX = (textureIndexes[cubeID][2] % 32)*16;
		texY = (textureIndexes[cubeID][2] / 32)*16;
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ+1.0, 1,0,0 , texX     ,texY     , lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, absY    , absZ+1.0, 1,0,0 , texX	   ,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, absY    , absZ    , 1,0,0 , texX+16.0,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ    , 1,0,0 , texX+16.0,texY     , lind,lind,lind,1.0));
	}
	if(!outOfBounds(x-1,y,z) && getCube(x-1,y,z).id == 0) { // right face
		lind = getCube(x-1,y,z).light/LIGHTMAX;
		texX = (textureIndexes[cubeID][3] % 32)*16;
		texY = (textureIndexes[cubeID][3] / 32)*16;
		renderData.push_back(Vertex(absX    , absY+1.0, absZ    , -1,0,0, texX     ,texY     , lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ    , -1,0,0, texX	   ,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ+1.0, -1,0,0, texX+16.0,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , absY+1.0, absZ+1.0, -1,0,0, texX+16.0,texY     , lind,lind,lind,1.0));
	}
	if(!outOfBounds(x,y-1,z) && getCube(x,y-1,z).id == 0) { // bottom face
		lind = getCube(x,y-1,z).light/LIGHTMAX;
		texX = (textureIndexes[cubeID][4] % 32)*16;
		texY = (textureIndexes[cubeID][4] / 32)*16;
		renderData.push_back(Vertex(absX+1.0, absY    , absZ+1.0, 0,-1,0, texX     ,texY     , lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ+1.0, 0,-1,0, texX     ,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ    , 0,-1,0, texX+16.0,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, absY    , absZ    , 0,-1,0, texX+16.0,texY     , lind,lind,lind,1.0));
	}
	if(!outOfBounds(x,y+1,z) && getCube(x,y+1,z).id == 0) { // top face
		lind = getCube(x,y+1,z).light/LIGHTMAX;
		texX = (textureIndexes[cubeID][5] % 32)*16;
		texY = (textureIndexes[cubeID][5] / 32)*16;
		renderData.push_back(Vertex(absX    , absY+1.0, absZ+1.0, 0,1,0 , texX     ,texY     , lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ+1.0, 0,1,0 , texX     ,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ    , 0,1,0 , texX+16.0,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , absY+1.0, absZ    , 0,1,0 , texX+16.0,texY     , lind,lind,lind,1.0));
	}
}

void Chunk::makeVbo() {
	glGenBuffers(1, (GLuint*) &VBOID);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glBufferData(GL_ARRAY_BUFFER, renderData.size()*sizeof(Vertex), &renderData[0], GL_STATIC_DRAW);
}

const int Chunk::textureIndexes[5][6] = { //order is front, back, left, right, bottom, top
										  {0,0,0,0,0,0}, //0 = air (empty, will never be used)
										  {2,2,2,2,2,2}, //1 = dirt
										  {3,3,3,3,3,3}, //2 = stone
										  {0,0,0,0,2,1}, //3 = grass
										  {4,4,4,4,4,4}  //4 = lolwtf
										};
