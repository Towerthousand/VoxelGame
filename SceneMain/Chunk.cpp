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
	if(XPOS != 0 && ZPOS != 0 && YPOS != 0 &&
	   XPOS != WORLDWIDTH-1 && ZPOS != WORLDWIDTH-1 && YPOS != WORLDHEIGHT-1)
		populate();
}

Chunk::~Chunk() {
}

void Chunk::populate() {
//	for(int x = 0; x < CHUNKSIZE; ++x) {
//		for(int z = 0; z < CHUNKSIZE; ++z) {
//			for(int y = 0; y < CHUNKSIZE; ++y) {
//				cubes[x][y][z] = Cube(2,0); //world is solid rock cube
//			}
//		}
//	}
	for(int x = CHUNKSIZE*(0.5/4.0); x < CHUNKSIZE*(3.5/4.0); ++x) {
		for(int z = CHUNKSIZE*(0.5/4.0); z < CHUNKSIZE*(3.5/4.0); ++z) {
			for(int y = CHUNKSIZE*(0.5/4.0); y < CHUNKSIZE*(3.5/4.0); ++y) {
				cubes[x][y][z] = Cube(2,0); //1 rock cube/chunk, of 3/4 CHUNKSIZE on edge
			}
		}
	}
	markedForRedraw = true;
}

bool Chunk::outOfBounds(int x, int y, int z) {
	return parentWorld.outOfBounds(x+(XPOS*CHUNKSIZE),y+(YPOS*CHUNKSIZE),z+(ZPOS*CHUNKSIZE));
}

Cube Chunk::getCube(int x, int y, int z) {
	if (x >= CHUNKSIZE || y >= CHUNKSIZE || z >= CHUNKSIZE || x < 0 || z < 0 || y < 0) //outside of this chunk
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
					cubeID = getCube(x,y,z).ID;
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
	float lindA,lindB,lindC,lindD = 1.0;
	//STRUCTURE PER VERTEX: Vx,Vy,Vz,
	//						Nx,Ny,Nz,
	//						Tx,Ty,
	//						Cr,Cg,Cb,Ca
	if(getCube(x,y,z+1).ID == 0) { // front face
		texX = (textureIndexes[cubeID][0] % 32)*16; // 4 = number of textures/row, 16 = width
		texY = (textureIndexes[cubeID][0] / 32)*16; // 4 = number of textures/row, 16 = height
		lindA = (getCube(x,y,z+1).light + getCube(x,y+1,z+1).light +
				 getCube(x-1,y,z+1).light + getCube(x-1,y+1,z+1).light)/4.0;
		lindB = (getCube(x,y,z+1).light + getCube(x,y-1,z+1).light +
				 getCube(x-1,y,z+1).light + getCube(x-1,y-1,z+1).light)/4.0;
		lindC = (getCube(x,y,z+1).light + getCube(x,y-1,z+1).light +
				 getCube(x+1,y,z+1).light + getCube(x+1,y-1,z+1).light)/4.0;
		lindD = (getCube(x,y,z+1).light + getCube(x,y+1,z+1).light +
				 getCube(x+1,y,z+1).light + getCube(x+1,y+1,z+1).light)/4.0;
		lindA = std::fmax(lindA,MINLIGHT)/MAXLIGHT;
		lindB = std::fmax(lindB,MINLIGHT)/MAXLIGHT;
		lindC = std::fmax(lindC,MINLIGHT)/MAXLIGHT;
		lindD = std::fmax(lindD,MINLIGHT)/MAXLIGHT;
		renderData.push_back(Vertex(absX    , absY+1.0, absZ+1.0, 0,0,1 , texX     ,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ+1.0, 0,0,1 , texX	   ,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX+1.0, absY    , absZ+1.0, 0,0,1 , texX+16.0,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ+1.0, 0,0,1 , texX+16.0,texY     , lindD,lindD,lindD,1.0));
	}
	if(getCube(x,y,z-1).ID == 0) { // back face
		lindA = (getCube(x,y,z-1).light + getCube(x,y+1,z-1).light +
				 getCube(x+1,y,z-1).light + getCube(x+1,y+1,z-1).light)/4.0;
		lindB = (getCube(x,y,z-1).light + getCube(x,y-1,z-1).light +
				 getCube(x+1,y,z-1).light + getCube(x+1,y-1,z-1).light)/4.0;
		lindC = (getCube(x,y,z-1).light + getCube(x,y-1,z-1).light +
				 getCube(x-1,y,z-1).light + getCube(x-1,y-1,z-1).light)/4.0;
		lindD = (getCube(x,y,z-1).light + getCube(x,y+1,z-1).light +
				 getCube(x-1,y,z-1).light + getCube(x-1,y+1,z-1).light)/4.0;
		lindA = std::fmax(lindA,MINLIGHT)/MAXLIGHT;
		lindB = std::fmax(lindB,MINLIGHT)/MAXLIGHT;
		lindC = std::fmax(lindC,MINLIGHT)/MAXLIGHT;
		lindD = std::fmax(lindD,MINLIGHT)/MAXLIGHT;
		texX = (textureIndexes[cubeID][1] % 32)*16;
		texY = (textureIndexes[cubeID][1] / 32)*16;
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ    , 0,0,-1, texX     ,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX+1.0, absY    , absZ    , 0,0,-1, texX	   ,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ    , 0,0,-1, texX+16.0,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX    , absY+1.0, absZ    , 0,0,-1, texX+16.0,texY     , lindD,lindD,lindD,1.0));
	}
	if(getCube(x+1,y,z).ID == 0) { // left face
		lindA = (getCube(x+1,y,z).light + getCube(x+1,y+1,z).light +
				 getCube(x+1,y,z+1).light + getCube(x+1,y+1,z+1).light)/4.0;
		lindB = (getCube(x+1,y,z).light + getCube(x+1,y-1,z).light +
				 getCube(x+1,y,z+1).light + getCube(x+1,y-1,z+1).light)/4.0;
		lindC = (getCube(x+1,y,z).light + getCube(x+1,y-1,z).light +
				 getCube(x+1,y,z-1).light + getCube(x+1,y-1,z-1).light)/4.0;
		lindD = (getCube(x+1,y,z).light + getCube(x+1,y+1,z).light +
				 getCube(x+1,y,z-1).light + getCube(x+1,y+1,z-1).light)/4.0;
		lindA = std::fmax(lindA,MINLIGHT)/MAXLIGHT;
		lindB = std::fmax(lindB,MINLIGHT)/MAXLIGHT;
		lindC = std::fmax(lindC,MINLIGHT)/MAXLIGHT;
		lindD = std::fmax(lindD,MINLIGHT)/MAXLIGHT;
		texX = (textureIndexes[cubeID][2] % 32)*16;
		texY = (textureIndexes[cubeID][2] / 32)*16;
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ+1.0, 1,0,0 , texX     ,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX+1.0, absY    , absZ+1.0, 1,0,0 , texX	   ,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX+1.0, absY    , absZ    , 1,0,0 , texX+16.0,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ    , 1,0,0 , texX+16.0,texY     , lindD,lindD,lindD,1.0));
	}
	if(getCube(x-1,y,z).ID == 0) { // right face
		lindA = (getCube(x-1,y,z).light + getCube(x-1,y+1,z).light +
				 getCube(x-1,y,z-1).light + getCube(x-1,y+1,z-1).light)/4.0;
		lindB = (getCube(x-1,y,z).light + getCube(x-1,y-1,z).light +
				 getCube(x-1,y,z-1).light + getCube(x-1,y-1,z-1).light)/4.0;
		lindC = (getCube(x-1,y,z).light + getCube(x-1,y-1,z).light +
				 getCube(x-1,y,z+1).light + getCube(x-1,y-1,z+1).light)/4.0;
		lindD = (getCube(x-1,y,z).light + getCube(x-1,y+1,z).light +
				 getCube(x-1,y,z+1).light + getCube(x-1,y+1,z+1).light)/4.0;
		lindA = std::fmax(lindA,MINLIGHT)/MAXLIGHT;
		lindB = std::fmax(lindB,MINLIGHT)/MAXLIGHT;
		lindC = std::fmax(lindC,MINLIGHT)/MAXLIGHT;
		lindD = std::fmax(lindD,MINLIGHT)/MAXLIGHT;
		texX = (textureIndexes[cubeID][3] % 32)*16;
		texY = (textureIndexes[cubeID][3] / 32)*16;
		renderData.push_back(Vertex(absX    , absY+1.0, absZ    , -1,0,0, texX     ,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ    , -1,0,0, texX	   ,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ+1.0, -1,0,0, texX+16.0,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX    , absY+1.0, absZ+1.0, -1,0,0, texX+16.0,texY     , lindD,lindD,lindD,1.0));
	}
	if(getCube(x,y-1,z).ID == 0) { // bottom face
		lindA = (getCube(x,y-1,z).light + getCube(x+1,y-1,z).light +
				 getCube(x,y-1,z+1).light + getCube(x+1,y-1,z+1).light)/4.0;
		lindB = (getCube(x,y-1,z).light + getCube(x-1,y-1,z).light +
				 getCube(x,y-1,z+1).light + getCube(x-1,y-1,z+1).light)/4.0;
		lindC = (getCube(x,y-1,z).light + getCube(x-1,y-1,z).light +
				 getCube(x,y-1,z-1).light + getCube(x-1,y-1,z-1).light)/4.0;
		lindD = (getCube(x,y-1,z).light + getCube(x+1,y-1,z).light +
				 getCube(x,y-1,z-1).light + getCube(x+1,y-1,z-1).light)/4.0;
		lindA = std::fmax(lindA,MINLIGHT)/MAXLIGHT;
		lindB = std::fmax(lindB,MINLIGHT)/MAXLIGHT;
		lindC = std::fmax(lindC,MINLIGHT)/MAXLIGHT;
		lindD = std::fmax(lindD,MINLIGHT)/MAXLIGHT;
		texX = (textureIndexes[cubeID][4] % 32)*16;
		texY = (textureIndexes[cubeID][4] / 32)*16;
		renderData.push_back(Vertex(absX+1.0, absY    , absZ+1.0, 0,-1,0, texX     ,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ+1.0, 0,-1,0, texX     ,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ    , 0,-1,0, texX+16.0,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX+1.0, absY    , absZ    , 0,-1,0, texX+16.0,texY     , lindD,lindD,lindD,1.0));
	}
	if(getCube(x,y+1,z).ID == 0) { // top face
		lindA = (getCube(x,y+1,z).light + getCube(x-1,y+1,z).light +
				 getCube(x,y+1,z+1).light + getCube(x-1,y+1,z+1).light)/4.0;
		lindB = (getCube(x,y+1,z).light + getCube(x+1,y+1,z).light +
				 getCube(x,y+1,z+1).light + getCube(x+1,y+1,z+1).light)/4.0;
		lindC = (getCube(x,y+1,z).light + getCube(x+1,y+1,z).light +
				 getCube(x,y+1,z-1).light + getCube(x+1,y+1,z-1).light)/4.0;
		lindD = (getCube(x,y+1,z).light + getCube(x-1,y+1,z).light +
				 getCube(x,y+1,z-1).light + getCube(x-1,y+1,z-1).light)/4.0;
		lindA = std::fmax(lindA,MINLIGHT)/MAXLIGHT;
		lindB = std::fmax(lindB,MINLIGHT)/MAXLIGHT;
		lindC = std::fmax(lindC,MINLIGHT)/MAXLIGHT;
		lindD = std::fmax(lindD,MINLIGHT)/MAXLIGHT;
		texX = (textureIndexes[cubeID][5] % 32)*16;
		texY = (textureIndexes[cubeID][5] / 32)*16;
		renderData.push_back(Vertex(absX    , absY+1.0, absZ+1.0, 0,1,0 , texX     ,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ+1.0, 0,1,0 , texX     ,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ    , 0,1,0 , texX+16.0,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX    , absY+1.0, absZ    , 0,1,0 , texX+16.0,texY     , lindD,lindD,lindD,1.0));
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
