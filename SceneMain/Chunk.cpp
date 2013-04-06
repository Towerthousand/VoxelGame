#include "Chunk.hpp"
#include "World.hpp"

Chunk::Chunk(int x, int y, int z, World &world) :
	outOfView(false), markedForRedraw(false),
	cubes(CHUNKSIZE,std::vector<std::vector<Cube> >
		  (CHUNKSIZE,std::vector<Cube>
		   (CHUNKSIZE,Cube(0,MINLIGHT)))),
	XPOS(x), YPOS(y), ZPOS(z),
	VBOID(1),
	parentWorld(world) {
}

Chunk::~Chunk() {
}

bool Chunk::getOutOfBounds(int x, int y, int z) const {
	return parentWorld.getOutOfBounds(x+(XPOS*CHUNKSIZE),y+(YPOS*CHUNKSIZE),z+(ZPOS*CHUNKSIZE));
}

Cube Chunk::getCube(int x, int y, int z) const {
	if (x >= CHUNKSIZE || y >= CHUNKSIZE || z >= CHUNKSIZE || x < 0 || z < 0 || y < 0) //outside of this chunk
		return parentWorld.getCubeAbs(x+(XPOS*CHUNKSIZE),y+(YPOS*CHUNKSIZE),z+(ZPOS*CHUNKSIZE));
	return cubes[x][y][z]; //inside current chunk
}

bool Chunk::getSkyAccess(int x,int y, int z) const {
	return parentWorld.getSkyAccess(x,y,z);
}

void Chunk::update(float deltaTime) {
	//empty arrays and re-do them
	markedForRedraw = false;
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


void Chunk::draw() const {
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glPushMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	glNormalPointer(GL_FLOAT, sizeof(Vertex),(GLvoid*)(4*sizeof(float)));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid*)(8*sizeof(float)));
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), (GLvoid*)(12*sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, renderData.size());

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Chunk::pushCubeToArray(int x,int y, int z,int cubeID) { //I DON'T KNOW HOW TO MAKE THIS COMPACT
	int absX = x+CHUNKSIZE*XPOS;
	int absY = y+CHUNKSIZE*YPOS;
	int absZ = z+CHUNKSIZE*ZPOS;
	int texY, texX;
	float lindA = 1.0,lindB = 1.0,lindC = 1.0,lindD = 1.0, lindE = 1.0;
	//STRUCTURE PER VERTEX: Vx,Vy,Vz,
	//						Nx,Ny,Nz,
	//						Tx,Ty,
	//						Cr,Cg,Cb,Ca
	if(getCube(x,y,z+1).ID == 0) { // front face
		texX = (textureIndexes[cubeID][0] % 32)*16; // 32 = number of textures/row, 16 = width
		texY = (textureIndexes[cubeID][0] / 32)*16; // 32 = number of textures/row, 16 = height
		if (cubeID != 4) {
			//if it's not a light (light should be fully lit) calculate the average of the adjacent
			//air blocks and assign max(max(average,adjacentBlock.light/2),MINLIGHT)
			lindA = (getCube(x,y,z+1).light + getCube(x,y+1,z+1).light +
					 getCube(x-1,y,z+1).light + getCube(x-1,y+1,z+1).light)/4.0;
			lindB = (getCube(x,y,z+1).light + getCube(x,y-1,z+1).light +
					 getCube(x-1,y,z+1).light + getCube(x-1,y-1,z+1).light)/4.0;
			lindC = (getCube(x,y,z+1).light + getCube(x,y-1,z+1).light +
					 getCube(x+1,y,z+1).light + getCube(x+1,y-1,z+1).light)/4.0;
			lindD = (getCube(x,y,z+1).light + getCube(x,y+1,z+1).light +
					 getCube(x+1,y,z+1).light + getCube(x+1,y+1,z+1).light)/4.0;
			lindA = std::fmax(std::fmax(lindA,getCube(x,y,z+1).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindB = std::fmax(std::fmax(lindB,getCube(x,y,z+1).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindC = std::fmax(std::fmax(lindC,getCube(x,y,z+1).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindD = std::fmax(std::fmax(lindD,getCube(x,y,z+1).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindE = (lindA+lindB+lindC+lindD)/4.0;
		}
		//t1
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ+1.0, 0,0,-1, texX     ,texY     , lindD,lindD,lindD,1.0));
		renderData.push_back(Vertex(absX    , absY+1.0, absZ+1.0, 0,0,-1, texX+16.0,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX+0.5, absY+0.5, absZ+1.0, 0,0,-1, texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t2
		renderData.push_back(Vertex(absX+1.0, absY    , absZ+1.0, 0,0,-1, texX	   ,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ+1.0, 0,0,-1, texX     ,texY     , lindD,lindD,lindD,1.0));
		renderData.push_back(Vertex(absX+0.5, absY+0.5, absZ+1.0, 0,0,-1, texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t3
		renderData.push_back(Vertex(absX    , absY    , absZ+1.0, 0,0,-1, texX+16.0,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX+1.0, absY    , absZ+1.0, 0,0,-1, texX	   ,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX+0.5, absY+0.5, absZ+1.0, 0,0,-1, texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t4
		renderData.push_back(Vertex(absX    , absY+1.0, absZ+1.0, 0,0,-1, texX+16.0,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ+1.0, 0,0,-1, texX+16.0,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX+0.5, absY+0.5, absZ+1.0, 0,0,-1, texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
	}
	if(getCube(x,y,z-1).ID == 0) { // back face
		if (cubeID != 4) {
			lindA = (getCube(x,y,z-1).light + getCube(x,y+1,z-1).light +
					 getCube(x+1,y,z-1).light + getCube(x+1,y+1,z-1).light)/4.0;
			lindB = (getCube(x,y,z-1).light + getCube(x,y-1,z-1).light +
					 getCube(x+1,y,z-1).light + getCube(x+1,y-1,z-1).light)/4.0;
			lindC = (getCube(x,y,z-1).light + getCube(x,y-1,z-1).light +
					 getCube(x-1,y,z-1).light + getCube(x-1,y-1,z-1).light)/4.0;
			lindD = (getCube(x,y,z-1).light + getCube(x,y+1,z-1).light +
					 getCube(x-1,y,z-1).light + getCube(x-1,y+1,z-1).light)/4.0;
			lindA = std::fmax(std::fmax(lindA,getCube(x,y,z-1).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindB = std::fmax(std::fmax(lindB,getCube(x,y,z-1).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindC = std::fmax(std::fmax(lindC,getCube(x,y,z-1).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindD = std::fmax(std::fmax(lindD,getCube(x,y,z-1).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindE = (lindA+lindB+lindC+lindD)/4.0;
		}
		texX = (textureIndexes[cubeID][1] % 32)*16;
		texY = (textureIndexes[cubeID][1] / 32)*16;
		//t1
		renderData.push_back(Vertex(absX    , absY+1.0, absZ    , 0,0,-1, texX+16.0,texY     , lindD,lindD,lindD,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ    , 0,0,-1, texX     ,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX+0.5, absY+0.5, absZ    , 0,0,-1, texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t2
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ    , 0,0,-1, texX     ,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX+1.0, absY    , absZ    , 0,0,-1, texX	   ,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX+0.5, absY+0.5, absZ    , 0,0,-1, texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t3
		renderData.push_back(Vertex(absX+1.0, absY    , absZ    , 0,0,-1, texX	   ,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ    , 0,0,-1, texX+16.0,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX+0.5, absY+0.5, absZ    , 0,0,-1, texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t4
		renderData.push_back(Vertex(absX    , absY    , absZ    , 0,0,-1, texX+16.0,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX    , absY+1.0, absZ    , 0,0,-1, texX+16.0,texY     , lindD,lindD,lindD,1.0));
		renderData.push_back(Vertex(absX+0.5, absY+0.5, absZ    , 0,0,-1, texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
	}
	if(getCube(x+1,y,z).ID == 0) { // left face
		if (cubeID != 4) {
			lindA = (getCube(x+1,y,z).light + getCube(x+1,y+1,z).light +
					 getCube(x+1,y,z+1).light + getCube(x+1,y+1,z+1).light)/4.0;
			lindB = (getCube(x+1,y,z).light + getCube(x+1,y-1,z).light +
					 getCube(x+1,y,z+1).light + getCube(x+1,y-1,z+1).light)/4.0;
			lindC = (getCube(x+1,y,z).light + getCube(x+1,y-1,z).light +
					 getCube(x+1,y,z-1).light + getCube(x+1,y-1,z-1).light)/4.0;
			lindD = (getCube(x+1,y,z).light + getCube(x+1,y+1,z).light +
					 getCube(x+1,y,z-1).light + getCube(x+1,y+1,z-1).light)/4.0;
			lindA = std::fmax(std::fmax(lindA,getCube(x+1,y,z).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindB = std::fmax(std::fmax(lindB,getCube(x+1,y,z).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindC = std::fmax(std::fmax(lindC,getCube(x+1,y,z).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindD = std::fmax(std::fmax(lindD,getCube(x+1,y,z).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindE = (lindA+lindB+lindC+lindD)/4.0;
		}
		texX = (textureIndexes[cubeID][2] % 32)*16;
		texY = (textureIndexes[cubeID][2] / 32)*16;
		//t1
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ    , -1,0,0, texX     ,texY     , lindD,lindD,lindD,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ+1.0, -1,0,0, texX+16.0,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+0.5, absZ+0.5, -1,0,0, texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t2
		renderData.push_back(Vertex(absX+1.0, absY    , absZ    , -1,0,0, texX	   ,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ    , -1,0,0, texX     ,texY     , lindD,lindD,lindD,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+0.5, absZ+0.5, -1,0,0, texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t3
		renderData.push_back(Vertex(absX+1.0, absY    , absZ+1.0, -1,0,0, texX+16.0,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX+1.0, absY    , absZ    , -1,0,0, texX	   ,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+0.5, absZ+0.5, -1,0,0, texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t4
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ+1.0, -1,0,0, texX+16.0,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX+1.0, absY    , absZ+1.0, -1,0,0, texX+16.0,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+0.5, absZ+0.5, -1,0,0, texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
	}
	if(getCube(x-1,y,z).ID == 0) { // right face
		if (cubeID != 4) {
			lindA = (getCube(x-1,y,z).light + getCube(x-1,y+1,z).light +
					 getCube(x-1,y,z-1).light + getCube(x-1,y+1,z-1).light)/4.0;
			lindB = (getCube(x-1,y,z).light + getCube(x-1,y-1,z).light +
					 getCube(x-1,y,z-1).light + getCube(x-1,y-1,z-1).light)/4.0;
			lindC = (getCube(x-1,y,z).light + getCube(x-1,y-1,z).light +
					 getCube(x-1,y,z+1).light + getCube(x-1,y-1,z+1).light)/4.0;
			lindD = (getCube(x-1,y,z).light + getCube(x-1,y+1,z).light +
					 getCube(x-1,y,z+1).light + getCube(x-1,y+1,z+1).light)/4.0;
			lindA = std::fmax(std::fmax(lindA,getCube(x-1,y,z).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindB = std::fmax(std::fmax(lindB,getCube(x-1,y,z).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindC = std::fmax(std::fmax(lindC,getCube(x-1,y,z).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindD = std::fmax(std::fmax(lindD,getCube(x-1,y,z).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindE = (lindA+lindB+lindC+lindD)/4.0;
		}
		texX = (textureIndexes[cubeID][3] % 32)*16;
		texY = (textureIndexes[cubeID][3] / 32)*16;
		//t1
		renderData.push_back(Vertex(absX    , absY+1.0, absZ+1.0, -1,0,0, texX+16.0,texY     , lindD,lindD,lindD,1.0));
		renderData.push_back(Vertex(absX    , absY+1.0, absZ    , -1,0,0, texX     ,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX    , absY+0.5, absZ+0.5, -1,0,0, texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t2
		renderData.push_back(Vertex(absX    , absY+1.0, absZ    , -1,0,0, texX     ,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ    , -1,0,0, texX	   ,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX    , absY+0.5, absZ+0.5, -1,0,0, texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t3
		renderData.push_back(Vertex(absX    , absY    , absZ    , -1,0,0, texX	   ,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ+1.0, -1,0,0, texX+16.0,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX    , absY+0.5, absZ+0.5, -1,0,0, texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t4
		renderData.push_back(Vertex(absX    , absY    , absZ+1.0, -1,0,0, texX+16.0,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX    , absY+1.0, absZ+1.0, -1,0,0, texX+16.0,texY     , lindD,lindD,lindD,1.0));
		renderData.push_back(Vertex(absX    , absY+0.5, absZ+0.5, -1,0,0, texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
	}
	if(getCube(x,y-1,z).ID == 0) { // bottom face
		if (cubeID != 4) {
			lindA = (getCube(x,y-1,z).light + getCube(x+1,y-1,z).light +
					 getCube(x,y-1,z+1).light + getCube(x+1,y-1,z+1).light)/4.0;
			lindB = (getCube(x,y-1,z).light + getCube(x-1,y-1,z).light +
					 getCube(x,y-1,z+1).light + getCube(x-1,y-1,z+1).light)/4.0;
			lindC = (getCube(x,y-1,z).light + getCube(x-1,y-1,z).light +
					 getCube(x,y-1,z-1).light + getCube(x-1,y-1,z-1).light)/4.0;
			lindD = (getCube(x,y-1,z).light + getCube(x+1,y-1,z).light +
					 getCube(x,y-1,z-1).light + getCube(x+1,y-1,z-1).light)/4.0;
			lindA = std::fmax(std::fmax(lindA,getCube(x,y-1,z).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindB = std::fmax(std::fmax(lindB,getCube(x,y-1,z).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindC = std::fmax(std::fmax(lindC,getCube(x,y-1,z).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindD = std::fmax(std::fmax(lindD,getCube(x,y-1,z).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindE = (lindA+lindB+lindC+lindD)/4.0;
		}
		texX = (textureIndexes[cubeID][4] % 32)*16;
		texY = (textureIndexes[cubeID][4] / 32)*16;
		//t1
		renderData.push_back(Vertex(absX    , absY    , absZ    , 0,-1,0, texX+16.0,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX+1.0, absY    , absZ    , 0,-1,0, texX+16.0,texY     , lindD,lindD,lindD,1.0));
		renderData.push_back(Vertex(absX+0.5, absY    , absZ+0.5, 0,-1,0 ,texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t2
		renderData.push_back(Vertex(absX+1.0, absY    , absZ    , 0,-1,0, texX+16.0,texY     , lindD,lindD,lindD,1.0));
		renderData.push_back(Vertex(absX+1.0, absY    , absZ+1.0, 0,-1,0, texX     ,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX+0.5, absY    , absZ+0.5, 0,-1,0 ,texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t3
		renderData.push_back(Vertex(absX+1.0, absY    , absZ+1.0, 0,-1,0, texX     ,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ+1.0, 0,-1,0, texX     ,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX+0.5, absY    , absZ+0.5, 0,-1,0 ,texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t4
		renderData.push_back(Vertex(absX    , absY    , absZ+1.0, 0,-1,0, texX     ,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX    , absY    , absZ    , 0,-1,0, texX+16.0,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX+0.5, absY    , absZ+0.5, 0,-1,0 ,texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
	}
	if(getCube(x,y+1,z).ID == 0) { // top face
		if (cubeID != 4) {
			lindA = (getCube(x,y+1,z).light + getCube(x-1,y+1,z).light +
					 getCube(x,y+1,z+1).light + getCube(x-1,y+1,z+1).light)/4.0;
			lindB = (getCube(x,y+1,z).light + getCube(x+1,y+1,z).light +
					 getCube(x,y+1,z+1).light + getCube(x+1,y+1,z+1).light)/4.0;
			lindC = (getCube(x,y+1,z).light + getCube(x+1,y+1,z).light +
					 getCube(x,y+1,z-1).light + getCube(x+1,y+1,z-1).light)/4.0;
			lindD = (getCube(x,y+1,z).light + getCube(x-1,y+1,z).light +
					 getCube(x,y+1,z-1).light + getCube(x-1,y+1,z-1).light)/4.0;
			lindA = std::fmax(std::fmax(lindA,getCube(x,y+1,z).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindB = std::fmax(std::fmax(lindB,getCube(x,y+1,z).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindC = std::fmax(std::fmax(lindC,getCube(x,y+1,z).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindD = std::fmax(std::fmax(lindD,getCube(x,y+1,z).light/3.0),MINLIGHT)/(MAXLIGHT);
			lindE = (lindA+lindB+lindC+lindD)/4.0;
		}
		texX = (textureIndexes[cubeID][5] % 32)*16;
		texY = (textureIndexes[cubeID][5] / 32)*16;
		//t1
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ    , 0,1,0 , texX+16.0,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX    , absY+1.0, absZ    , 0,1,0 , texX+16.0,texY     , lindD,lindD,lindD,1.0));
		renderData.push_back(Vertex(absX+0.5, absY+1.0, absZ+0.5, 0,1,0 , texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t2
		renderData.push_back(Vertex(absX    , absY+1.0, absZ    , 0,1,0 , texX+16.0,texY     , lindD,lindD,lindD,1.0));
		renderData.push_back(Vertex(absX    , absY+1.0, absZ+1.0, 0,1,0 , texX     ,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX+0.5, absY+1.0, absZ+0.5, 0,1,0 , texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t3
		renderData.push_back(Vertex(absX    , absY+1.0, absZ+1.0, 0,1,0 , texX     ,texY     , lindA,lindA,lindA,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ+1.0, 0,1,0 , texX     ,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX+0.5, absY+1.0, absZ+0.5, 0,1,0 , texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
		//t4
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ+1.0, 0,1,0 , texX     ,texY+16.0, lindB,lindB,lindB,1.0));
		renderData.push_back(Vertex(absX+1.0, absY+1.0, absZ    , 0,1,0 , texX+16.0,texY+16.0, lindC,lindC,lindC,1.0));
		renderData.push_back(Vertex(absX+0.5, absY+1.0, absZ+0.5, 0,1,0 , texX+8.0 ,texY+8.0 , lindE,lindE,lindE,1.0));
	}
}

void Chunk::makeVbo() {
	glGenBuffers(1, (GLuint*) &VBOID);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glBufferData(GL_ARRAY_BUFFER, renderData.size()*sizeof(Vertex), &renderData[0], GL_STATIC_DRAW);
}

const int Chunk::textureIndexes[9][6] = { //order is front, back, left, right, bottom, top
										  {0,0,0,0,0,0}, //0 = air (empty, will never be used)
										  {2,2,2,2,2,2}, //1 = dirt
										  {3,3,3,3,3,3}, //2 = stone
										  {0,0,0,0,2,1}, //3 = grass
										  {4,4,4,4,4,4}, //4 = LIGHT
										  {5,5,5,5,5,5}, //5 = cobble
										  {6,6,6,6,7,7}, //6 = log
										  {8,8,8,8,8,8}, //7 = planks
										  {9,9,9,9,9,9}  //8 = sand
										};
