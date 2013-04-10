#include "Chunk.hpp"
#include "World.hpp"

Chunk::Chunk(int x, int y, int z, World &world) :
	outOfView(false), markedForRedraw(false),
	cubes(CHUNKSIZE,std::vector<std::vector<Cube> >
		  (CHUNKSIZE,std::vector<Cube>
		   (CHUNKSIZE,Cube(0,MINLIGHT)))),
	vertexCount(0),
	XPOS(x), YPOS(y), ZPOS(z),
	VBOID(1),
	parentWorld(world) {
	glGenBuffers(1, (GLuint*) &VBOID);
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
	std::vector<Vertex> renderData;
	int cubeID;
	for(int z = 0; z < CHUNKSIZE; ++z) {
		for(int y = 0; y < CHUNKSIZE; ++y) {
			for(int x = 0; x < CHUNKSIZE; ++x) {
				cubeID = getCube(x,y,z).ID;
				if (cubeID != 0) { // only draw if it's not air
					pushCubeToArray(x,y,z,cubeID,renderData);
				}
			}
		}
	}
	makeVbo(renderData);
	vertexCount = renderData.size();
}


void Chunk::draw() const {
	if(vertexCount != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, VBOID);
		glPushMatrix();
		glTranslatef(XPOS*CHUNKSIZE,YPOS*CHUNKSIZE,ZPOS*CHUNKSIZE);
		glScalef(0.5,0.5,0.5);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(3, GL_SHORT, sizeof(Vertex), 0);
		glTexCoordPointer(2, GL_SHORT, sizeof(Vertex), (GLvoid*)(3*sizeof(short)));
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), (GLvoid*)(2*sizeof(short)+3*sizeof(short)));

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glPopMatrix();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void Chunk::drawBoundingBox() const {
	glPushMatrix();
	glTranslatef(XPOS*CHUNKSIZE,YPOS*CHUNKSIZE,ZPOS*CHUNKSIZE);
	glScalef(CHUNKSIZE,CHUNKSIZE,CHUNKSIZE);
	glColor4f(1,0,0,1);
	glBegin(GL_QUADS);
	glVertex3f(1, 0, 0);
	glVertex3f(1, 0, 1);
	glVertex3f(0, 0, 1);
	glVertex3f(0, 0, 0);

	glVertex3f(0, 1, 0);
	glVertex3f(0, 1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(1, 1, 0);

	glVertex3f(0, 0, 1);
	glVertex3f(0, 1, 1);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 0, 0);

	glVertex3f(1, 0, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(1, 1, 1);
	glVertex3f(1, 0, 1);

	glVertex3f(0, 1, 0);
	glVertex3f(1, 1, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(0, 0, 0);

	glVertex3f(0, 0, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(0, 1, 1);
	glEnd();

	glPopMatrix();
}

void Chunk::pushCubeToArray(short x,short y, short z,unsigned char cubeID, std::vector<Vertex> &renderData) { //I DON'T KNOW HOW TO MAKE THIS COMPACT
	short absX = 2*x;
	short absY = 2*y;
	short absZ = 2*z;
	short texY, texX;
	float lindAf = 1.0,lindBf = 1.0,lindCf = 1.0,lindDf = 1.0;
	unsigned char lindA = 255, lindB = 255, lindC = 255, lindD = 255, lindE = 255;
	//STRUCTURE PER VERTEX: Vx,Vy,Vz,
	//						Tx,Ty,
	//						Cr,Cg,Cb,Ca
	if(getCube(x,y,z+1).ID == 0) { // front face
		if (cubeID != 4) {
			//if it's not a light (light should be fully lit) calculate the average of the adjacent
			//air blocks and assign max(max(average,adjacentBlock.light/2),MINLIGHT)
			lindAf = (getCube(x,y,z+1).light + getCube(x,y+1,z+1).light +
					 getCube(x-1,y,z+1).light + getCube(x-1,y+1,z+1).light)/4.0; //between 0 and MAXLIGHT
			lindBf = (getCube(x,y,z+1).light + getCube(x,y-1,z+1).light +
					 getCube(x-1,y,z+1).light + getCube(x-1,y-1,z+1).light)/4.0;
			lindCf = (getCube(x,y,z+1).light + getCube(x,y-1,z+1).light +
					 getCube(x+1,y,z+1).light + getCube(x+1,y-1,z+1).light)/4.0;
			lindDf = (getCube(x,y,z+1).light + getCube(x,y+1,z+1).light +
					 getCube(x+1,y,z+1).light + getCube(x+1,y+1,z+1).light)/4.0;
			lindA = (std::fmax(std::fmax(lindAf,getCube(x,y,z+1).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindB = (std::fmax(std::fmax(lindBf,getCube(x,y,z+1).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindC = (std::fmax(std::fmax(lindCf,getCube(x,y,z+1).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindD = (std::fmax(std::fmax(lindDf,getCube(x,y,z+1).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindE = (lindA+lindB+lindC+lindD)/4; //between 0 and 255
		}
		texX = (textureIndexes[cubeID][0] % (512/TEXSIZE))*TEXSIZE; // TEXSIZE/2 = number of textures/row
		texY = (textureIndexes[cubeID][0] / (512/TEXSIZE))*TEXSIZE; // TEXSIZE/2 = number of textures/row
		//t1
		renderData.push_back(Vertex(absX+2, absY+2, absZ+2, texX          ,texY          , lindD,lindD,lindD,255));
		renderData.push_back(Vertex(absX  , absY+2, absZ+2, texX+TEXSIZE  ,texY          , lindA,lindA,lindA,255));
		renderData.push_back(Vertex(absX+1, absY+1, absZ+2, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t2
		renderData.push_back(Vertex(absX+2, absY  , absZ+2, texX	      ,texY+TEXSIZE  , lindC,lindC,lindC,255));
		renderData.push_back(Vertex(absX+2, absY+2, absZ+2, texX          ,texY          , lindD,lindD,lindD,255));
		renderData.push_back(Vertex(absX+1, absY+1, absZ+2, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t3
		renderData.push_back(Vertex(absX  , absY  , absZ+2, texX+TEXSIZE  ,texY+TEXSIZE  , lindB,lindB,lindB,255));
		renderData.push_back(Vertex(absX+2, absY  , absZ+2, texX	      ,texY+TEXSIZE  , lindC,lindC,lindC,255));
		renderData.push_back(Vertex(absX+1, absY+1, absZ+2, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t4
		renderData.push_back(Vertex(absX  , absY+2, absZ+2, texX+TEXSIZE  ,texY          , lindA,lindA,lindA,255));
		renderData.push_back(Vertex(absX  , absY  , absZ+2, texX+TEXSIZE  ,texY+TEXSIZE  , lindB,lindB,lindB,255));
		renderData.push_back(Vertex(absX+1, absY+1, absZ+2, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
	}
	if(getCube(x,y,z-1).ID == 0) { // back face
		if (cubeID != 4) {
			lindAf = (getCube(x,y,z-1).light + getCube(x,y+1,z-1).light +
					 getCube(x+1,y,z-1).light + getCube(x+1,y+1,z-1).light)/4.0;
			lindBf = (getCube(x,y,z-1).light + getCube(x,y-1,z-1).light +
					 getCube(x+1,y,z-1).light + getCube(x+1,y-1,z-1).light)/4.0;
			lindCf = (getCube(x,y,z-1).light + getCube(x,y-1,z-1).light +
					 getCube(x-1,y,z-1).light + getCube(x-1,y-1,z-1).light)/4.0;
			lindDf = (getCube(x,y,z-1).light + getCube(x,y+1,z-1).light +
					 getCube(x-1,y,z-1).light + getCube(x-1,y+1,z-1).light)/4.0;
			lindA = (std::fmax(std::fmax(lindAf,getCube(x,y,z-1).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindB = (std::fmax(std::fmax(lindBf,getCube(x,y,z-1).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindC = (std::fmax(std::fmax(lindCf,getCube(x,y,z-1).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindD = (std::fmax(std::fmax(lindDf,getCube(x,y,z-1).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindE = (lindA+lindB+lindC+lindD)/4;
		}
		texX = (textureIndexes[cubeID][1] % (512/TEXSIZE))*TEXSIZE;
		texY = (textureIndexes[cubeID][1] / (512/TEXSIZE))*TEXSIZE;
		//t1
		renderData.push_back(Vertex(absX  , absY+2, absZ, texX+TEXSIZE  ,texY          , lindD,lindD,lindD,255));
		renderData.push_back(Vertex(absX+2, absY+2, absZ, texX          ,texY          , lindA,lindA,lindA,255));
		renderData.push_back(Vertex(absX+1, absY+1, absZ, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t2
		renderData.push_back(Vertex(absX+2, absY+2, absZ, texX          ,texY          , lindA,lindA,lindA,255));
		renderData.push_back(Vertex(absX+2, absY  , absZ, texX	        ,texY+TEXSIZE  , lindB,lindB,lindB,255));
		renderData.push_back(Vertex(absX+1, absY+1, absZ, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t3
		renderData.push_back(Vertex(absX+2, absY  , absZ, texX	        ,texY+TEXSIZE  , lindB,lindB,lindB,255));
		renderData.push_back(Vertex(absX  , absY  , absZ, texX+TEXSIZE  ,texY+TEXSIZE  , lindC,lindC,lindC,255));
		renderData.push_back(Vertex(absX+1, absY+1, absZ, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t4
		renderData.push_back(Vertex(absX  , absY  , absZ, texX+TEXSIZE  ,texY+TEXSIZE  , lindC,lindC,lindC,255));
		renderData.push_back(Vertex(absX  , absY+2, absZ, texX+TEXSIZE  ,texY          , lindD,lindD,lindD,255));
		renderData.push_back(Vertex(absX+1, absY+1, absZ, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
	}
	if(getCube(x+1,y,z).ID == 0) { // left face
		if (cubeID != 4) {
			lindAf = (getCube(x+1,y,z).light + getCube(x+1,y+1,z).light +
					 getCube(x+1,y,z+1).light + getCube(x+1,y+1,z+1).light)/4.0;
			lindBf = (getCube(x+1,y,z).light + getCube(x+1,y-1,z).light +
					 getCube(x+1,y,z+1).light + getCube(x+1,y-1,z+1).light)/4.0;
			lindCf = (getCube(x+1,y,z).light + getCube(x+1,y-1,z).light +
					 getCube(x+1,y,z-1).light + getCube(x+1,y-1,z-1).light)/4.0;
			lindDf = (getCube(x+1,y,z).light + getCube(x+1,y+1,z).light +
					 getCube(x+1,y,z-1).light + getCube(x+1,y+1,z-1).light)/4.0;
			lindA = (std::fmax(std::fmax(lindAf,getCube(x+1,y,z).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindB = (std::fmax(std::fmax(lindBf,getCube(x+1,y,z).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindC = (std::fmax(std::fmax(lindCf,getCube(x+1,y,z).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindD = (std::fmax(std::fmax(lindDf,getCube(x+1,y,z).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindE = (lindA+lindB+lindC+lindD)/4;
		}
		texX = (textureIndexes[cubeID][2] % (512/TEXSIZE))*TEXSIZE;
		texY = (textureIndexes[cubeID][2] / (512/TEXSIZE))*TEXSIZE;
		//t1
		renderData.push_back(Vertex(absX+2, absY+2, absZ  , texX          ,texY          , lindD,lindD,lindD,255));
		renderData.push_back(Vertex(absX+2, absY+2, absZ+2, texX+TEXSIZE  ,texY          , lindA,lindA,lindA,255));
		renderData.push_back(Vertex(absX+2, absY+1, absZ+1, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t2
		renderData.push_back(Vertex(absX+2, absY  , absZ  , texX          ,texY+TEXSIZE  , lindC,lindC,lindC,255));
		renderData.push_back(Vertex(absX+2, absY+2, absZ  , texX          ,texY          , lindD,lindD,lindD,255));
		renderData.push_back(Vertex(absX+2, absY+1, absZ+1, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t3
		renderData.push_back(Vertex(absX+2, absY  , absZ+2, texX+TEXSIZE  ,texY+TEXSIZE  , lindB,lindB,lindB,255));
		renderData.push_back(Vertex(absX+2, absY  , absZ  , texX          ,texY+TEXSIZE  , lindC,lindC,lindC,255));
		renderData.push_back(Vertex(absX+2, absY+1, absZ+1, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t4
		renderData.push_back(Vertex(absX+2, absY+2, absZ+2, texX+TEXSIZE  ,texY          , lindA,lindA,lindA,255));
		renderData.push_back(Vertex(absX+2, absY  , absZ+2, texX+TEXSIZE  ,texY+TEXSIZE  , lindB,lindB,lindB,255));
		renderData.push_back(Vertex(absX+2, absY+1, absZ+1, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
	}
	if(getCube(x-1,y,z).ID == 0) { // right face
		if (cubeID != 4) {
			lindAf = (getCube(x-1,y,z).light + getCube(x-1,y+1,z).light +
					 getCube(x-1,y,z-1).light + getCube(x-1,y+1,z-1).light)/4.0;
			lindBf = (getCube(x-1,y,z).light + getCube(x-1,y-1,z).light +
					 getCube(x-1,y,z-1).light + getCube(x-1,y-1,z-1).light)/4.0;
			lindCf = (getCube(x-1,y,z).light + getCube(x-1,y-1,z).light +
					 getCube(x-1,y,z+1).light + getCube(x-1,y-1,z+1).light)/4.0;
			lindDf = (getCube(x-1,y,z).light + getCube(x-1,y+1,z).light +
					 getCube(x-1,y,z+1).light + getCube(x-1,y+1,z+1).light)/4.0;
			lindA = (std::fmax(std::fmax(lindAf,getCube(x-1,y,z).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindB = (std::fmax(std::fmax(lindBf,getCube(x-1,y,z).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindC = (std::fmax(std::fmax(lindCf,getCube(x-1,y,z).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindD = (std::fmax(std::fmax(lindDf,getCube(x-1,y,z).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindE = (lindA+lindB+lindC+lindD)/4;
		}
		texX = (textureIndexes[cubeID][3] % (512/TEXSIZE))*TEXSIZE;
		texY = (textureIndexes[cubeID][3] / (512/TEXSIZE))*TEXSIZE;
		//t1
		renderData.push_back(Vertex(absX, absY+2, absZ+2, texX+TEXSIZE  ,texY          , lindD,lindD,lindD,255));
		renderData.push_back(Vertex(absX, absY+2, absZ  , texX          ,texY          , lindA,lindA,lindA,255));
		renderData.push_back(Vertex(absX, absY+1, absZ+1, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t2
		renderData.push_back(Vertex(absX, absY+2, absZ  , texX          ,texY          , lindA,lindA,lindA,255));
		renderData.push_back(Vertex(absX, absY  , absZ  , texX	        ,texY+TEXSIZE  , lindB,lindB,lindB,255));
		renderData.push_back(Vertex(absX, absY+1, absZ+1, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t3
		renderData.push_back(Vertex(absX, absY  , absZ  , texX	        ,texY+TEXSIZE  , lindB,lindB,lindB,255));
		renderData.push_back(Vertex(absX, absY  , absZ+2, texX+TEXSIZE  ,texY+TEXSIZE  , lindC,lindC,lindC,255));
		renderData.push_back(Vertex(absX, absY+1, absZ+1, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t4
		renderData.push_back(Vertex(absX, absY  , absZ+2, texX+TEXSIZE  ,texY+TEXSIZE  , lindC,lindC,lindC,255));
		renderData.push_back(Vertex(absX, absY+2, absZ+2, texX+TEXSIZE  ,texY          , lindD,lindD,lindD,255));
		renderData.push_back(Vertex(absX, absY+1, absZ+1, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
	}
	if(getCube(x,y-1,z).ID == 0) { // bottom face
		if (cubeID != 4) {
			lindAf = (getCube(x,y-1,z).light + getCube(x+1,y-1,z).light +
					 getCube(x,y-1,z+1).light + getCube(x+1,y-1,z+1).light)/4.0;
			lindBf = (getCube(x,y-1,z).light + getCube(x-1,y-1,z).light +
					 getCube(x,y-1,z+1).light + getCube(x-1,y-1,z+1).light)/4.0;
			lindCf = (getCube(x,y-1,z).light + getCube(x-1,y-1,z).light +
					 getCube(x,y-1,z-1).light + getCube(x-1,y-1,z-1).light)/4.0;
			lindDf = (getCube(x,y-1,z).light + getCube(x+1,y-1,z).light +
					 getCube(x,y-1,z-1).light + getCube(x+1,y-1,z-1).light)/4.0;
			lindA = (std::fmax(std::fmax(lindAf,getCube(x,y-1,z).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindB = (std::fmax(std::fmax(lindBf,getCube(x,y-1,z).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindC = (std::fmax(std::fmax(lindCf,getCube(x,y-1,z).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindD = (std::fmax(std::fmax(lindDf,getCube(x,y-1,z).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindE = (lindA+lindB+lindC+lindD)/4;
		}
		texX = (textureIndexes[cubeID][4] % (512/TEXSIZE))*TEXSIZE;
		texY = (textureIndexes[cubeID][4] / (512/TEXSIZE))*TEXSIZE;
		//t1
		renderData.push_back(Vertex(absX  , absY, absZ  , texX+TEXSIZE  ,texY+TEXSIZE  , lindC,lindC,lindC,255));
		renderData.push_back(Vertex(absX+2, absY, absZ  , texX+TEXSIZE  ,texY          , lindD,lindD,lindD,255));
		renderData.push_back(Vertex(absX+1, absY, absZ+1, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t2
		renderData.push_back(Vertex(absX+2, absY, absZ  , texX+TEXSIZE  ,texY          , lindD,lindD,lindD,255));
		renderData.push_back(Vertex(absX+2, absY, absZ+2, texX          ,texY          , lindA,lindA,lindA,255));
		renderData.push_back(Vertex(absX+1, absY, absZ+1, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t3
		renderData.push_back(Vertex(absX+2, absY, absZ+2, texX          ,texY          , lindA,lindA,lindA,255));
		renderData.push_back(Vertex(absX  , absY, absZ+2, texX	        ,texY+TEXSIZE  , lindB,lindB,lindB,255));
		renderData.push_back(Vertex(absX+1, absY, absZ+1, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t4
		renderData.push_back(Vertex(absX  , absY, absZ+2, texX	        ,texY+TEXSIZE  , lindB,lindB,lindB,255));
		renderData.push_back(Vertex(absX  , absY, absZ  , texX+TEXSIZE  ,texY+TEXSIZE  , lindC,lindC,lindC,255));
		renderData.push_back(Vertex(absX+1, absY, absZ+1, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
	}
	if(getCube(x,y+1,z).ID == 0) { // top face
		if (cubeID != 4) {
			lindAf = (getCube(x,y+1,z).light + getCube(x-1,y+1,z).light +
					 getCube(x,y+1,z+1).light + getCube(x-1,y+1,z+1).light)/4.0;
			lindBf = (getCube(x,y+1,z).light + getCube(x+1,y+1,z).light +
					 getCube(x,y+1,z+1).light + getCube(x+1,y+1,z+1).light)/4.0;
			lindCf = (getCube(x,y+1,z).light + getCube(x+1,y+1,z).light +
					 getCube(x,y+1,z-1).light + getCube(x+1,y+1,z-1).light)/4.0;
			lindDf = (getCube(x,y+1,z).light + getCube(x-1,y+1,z).light +
					 getCube(x,y+1,z-1).light + getCube(x-1,y+1,z-1).light)/4.0;
			lindA = (std::fmax(std::fmax(lindAf,getCube(x,y+1,z).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindB = (std::fmax(std::fmax(lindBf,getCube(x,y+1,z).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindC = (std::fmax(std::fmax(lindCf,getCube(x,y+1,z).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindD = (std::fmax(std::fmax(lindDf,getCube(x,y+1,z).light/3.0),MINLIGHT)/(MAXLIGHT))*255;
			lindE = (lindA+lindB+lindC+lindD)/4;
		}
		texX = (textureIndexes[cubeID][5] % (512/TEXSIZE))*TEXSIZE;
		texY = (textureIndexes[cubeID][5] / (512/TEXSIZE))*TEXSIZE;
		//t1
		renderData.push_back(Vertex(absX+2, absY+2, absZ  , texX+TEXSIZE  ,texY+TEXSIZE  , lindC,lindC,lindC,255));
		renderData.push_back(Vertex(absX  , absY+2, absZ  , texX+TEXSIZE  ,texY          , lindD,lindD,lindD,255));
		renderData.push_back(Vertex(absX+1, absY+2, absZ+1, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t2
		renderData.push_back(Vertex(absX  , absY+2, absZ  , texX+TEXSIZE  ,texY          , lindD,lindD,lindD,255));
		renderData.push_back(Vertex(absX  , absY+2, absZ+2, texX          ,texY          , lindA,lindA,lindA,255));
		renderData.push_back(Vertex(absX+1, absY+2, absZ+1, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t3
		renderData.push_back(Vertex(absX  , absY+2, absZ+2, texX          ,texY          , lindA,lindA,lindA,255));
		renderData.push_back(Vertex(absX+2, absY+2, absZ+2, texX	      ,texY+TEXSIZE  , lindB,lindB,lindB,255));
		renderData.push_back(Vertex(absX+1, absY+2, absZ+1, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
		//t4
		renderData.push_back(Vertex(absX+2, absY+2, absZ+2, texX	      ,texY+TEXSIZE  , lindB,lindB,lindB,255));
		renderData.push_back(Vertex(absX+2, absY+2, absZ  , texX+TEXSIZE  ,texY+TEXSIZE  , lindC,lindC,lindC,255));
		renderData.push_back(Vertex(absX+1, absY+2, absZ+1, texX+TEXSIZE/2,texY+TEXSIZE/2, lindE,lindE,lindE,255));
	}
}

void Chunk::makeVbo(std::vector<Vertex> &renderData) {
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
