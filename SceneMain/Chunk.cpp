#include "Chunk.hpp"
#include "World.hpp"
#include <libnoise/noise.h>
#include "noiseutils.h"

Chunk::Chunk(int x, int z, int seed, World &world) : XPOS(x), ZPOS(z), SEED(seed), parentWorld(world){
	cubes = std::vector<std::vector<std::vector<int> > >
			(CHUNKWIDTH,std::vector<std::vector<int> >
			 (CHUNKHEIGHT,std::vector<int>
			  (CHUNKWIDTH,0)));
	VBOID = XPOS*WORLDSIZE+ZPOS+1;
	outOfView = false;
	markedForRedraw = false;
	grassTimer = 0.0;
	populate();
}

Chunk::~Chunk() {
}

void Chunk::populate() {
	//set everything below SEALEVEL to stone
	for(int x = 0; x < CHUNKWIDTH; ++x) {
		for(int z = 0; z < CHUNKWIDTH; ++z) {
			for(int y = 0; y < SEALEVEL; ++y) {
				setCube(x,y,z,2);
			}
		}
	}

	// GENERATE TERRAIN HEIGHTMAP
	module::Perlin myModule;
	myModule.SetSeed(SEED);
	myModule.SetOctaveCount(10);
	myModule.SetLacunarity(1);
	myModule.SetFrequency(1);
	myModule.SetPersistence(0.1);

	module::ScaleBias flatTerrain;
	flatTerrain.SetSourceModule (0, myModule);
	flatTerrain.SetScale (0.5);
	flatTerrain.SetBias(-0.95);

	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule (flatTerrain);
	heightMapBuilder.SetDestNoiseMap (heightMap);
	heightMapBuilder.SetDestSize (CHUNKWIDTH,CHUNKWIDTH);
	float scaleFactor = 2.0;
	heightMapBuilder.SetBounds ((XPOS)/scaleFactor, (XPOS+1)/scaleFactor,
								(ZPOS)/scaleFactor, (ZPOS+1)/scaleFactor);
	heightMapBuilder.Build ();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap (heightMap);
	renderer.SetDestImage (image);
	renderer.ClearGradient ();
	renderer.AddGradientPoint (-1.00, utils::Color (    0,  0,   0, 255));
	renderer.AddGradientPoint ( 1.00, utils::Color (  255,255, 255, 255));
	renderer.Render ();

	// populate acording to heightmap
	// everything is raised according to sealevel
	float index; // relative index from heightmap in float value (between 0 and CHUNKSIZE)
	int height;  // floored index
	for(int x = 0; x < CHUNKWIDTH; ++x) {
		for(int z = 0; z < CHUNKWIDTH; ++z) {
			for(int y = 0; y < CHUNKHEIGHT; ++y) {
				index = image.GetValue(x,z).red; //you should implement your own
				index /= 255.0;				     //fucking perl noise lib
				index *= CHUNKHEIGHT;
				height = floor(index);
				if (y < height-2 && y+SEALEVEL < CHUNKHEIGHT)
					setCube(x,y+SEALEVEL,z,2);
				else if (y <= height-1 && y+SEALEVEL < CHUNKHEIGHT)
					setCube(x,y+SEALEVEL,z,1);
				else if(y == height && y+SEALEVEL < CHUNKHEIGHT)
					setCube(x,y+SEALEVEL,z,3);
			}
		}
	}
	markedForRedraw = true;
}

void Chunk::setCube(int x, int y, int z, int id) {
	if (y < 0 || y >= CHUNKHEIGHT) //over the skylimit, under bedrock
		return;
	else if (x >= CHUNKWIDTH || z >= CHUNKWIDTH || x < 0 || z < 0)//outside of this chunk
		parentWorld.setCubeAbs(x+(XPOS*CHUNKWIDTH),y,z+(ZPOS*CHUNKWIDTH),id);
	else //inside this chunk
		cubes[x][y][z] = id;
	markedForRedraw = true;
}

int Chunk::getCube(int x, int y, int z) const {
	if (y < 0 || y >= CHUNKHEIGHT) //over the skylimit, under bedrock
		return 0;
	if (x >= CHUNKWIDTH || z >= CHUNKWIDTH || x < 0 || z < 0)//outside of this chunk
		return parentWorld.getCubeAbs(x+(XPOS*CHUNKWIDTH),y,z+(ZPOS*CHUNKWIDTH));
	return cubes[x][y][z]; //inside current chunk
}

void Chunk::update(float deltaTime) {
	updateGrass(deltaTime);
	if (markedForRedraw) { //empty arrays and re-do them
		markedForRedraw = false;
		renderData.resize(0);
		int cubeID;
		for(int z = 0; z < CHUNKWIDTH; ++z) {
			for(int y = 0; y < CHUNKHEIGHT; ++y) {
				for(int x = 0; x < CHUNKWIDTH; ++x) {
					cubeID = getCube(x,y,z);
					if (cubeID != 0) { // only draw if it's not air
						pushCubeToArray(x,y,z,cubeID);
					}
				}
			}
		}
		makeVbo();
	}
}

void Chunk::pushCubeToArray(int x,int y, int z,int cubeID) {
	int absX = x+CHUNKWIDTH*XPOS;
	int absZ = z+CHUNKWIDTH*ZPOS;
	int textureY, textureX;
	//STRUCTURE PER VERTEX: Vx,Vy,Vz,Nx,Ny,Nz,Tx,Ty
	if(getCube(x,y,z+1) == 0) { // front face
		textureX = (textureIndexes[cubeID][0] % 4)*16; // 4 = number of textures/row, 16 = width
		textureY = (textureIndexes[cubeID][0] / 4)*16; // 4 = number of textures/row, 16 = height
		renderData.push_back(Vertex(absX    , y+1.0, absZ+1.0, 0,0,1, textureX     ,textureY));
		renderData.push_back(Vertex(absX    , y    , absZ+1.0, 0,0,1, textureX	   ,textureY+16.0));
		renderData.push_back(Vertex(absX+1.0, y    , absZ+1.0, 0,0,1, textureX+16.0,textureY+16.0));
		renderData.push_back(Vertex(absX+1.0, y+1.0, absZ+1.0, 0,0,1, textureX+16.0,textureY));
	}
	if(getCube(x,y,z-1) == 0) { // back face
		textureX = (textureIndexes[cubeID][1] % 4)*16;
		textureY = (textureIndexes[cubeID][1] / 4)*16;
		renderData.push_back(Vertex(absX+1.0, y+1.0, absZ, 0,0,-1, textureX     ,textureY));
		renderData.push_back(Vertex(absX+1.0, y    , absZ, 0,0,-1, textureX	    ,textureY+16.0));
		renderData.push_back(Vertex(absX    , y    , absZ, 0,0,-1, textureX+16.0,textureY+16.0));
		renderData.push_back(Vertex(absX    , y+1.0, absZ, 0,0,-1, textureX+16.0,textureY));
	}
	if(getCube(x+1,y,z) == 0) { // left face
		textureX = (textureIndexes[cubeID][2] % 4)*16;
		textureY = (textureIndexes[cubeID][2] / 4)*16;
		renderData.push_back(Vertex(absX+1.0, y+1.0, absZ+1.0, 1,0,0, textureX     ,textureY));
		renderData.push_back(Vertex(absX+1.0, y    , absZ+1.0, 1,0,0, textureX	   ,textureY+16.0));
		renderData.push_back(Vertex(absX+1.0, y    , absZ    , 1,0,0, textureX+16.0,textureY+16.0));
		renderData.push_back(Vertex(absX+1.0, y+1.0, absZ    , 1,0,0, textureX+16.0,textureY));
	}
	if(getCube(x-1,y,z) == 0) { // right face
		textureX = (textureIndexes[cubeID][3] % 4)*16;
		textureY = (textureIndexes[cubeID][3] / 4)*16;
		renderData.push_back(Vertex(absX    , y+1.0, absZ    , -1,0,0, textureX     ,textureY));
		renderData.push_back(Vertex(absX    , y    , absZ    , -1,0,0, textureX	    ,textureY+16.0));
		renderData.push_back(Vertex(absX    , y    , absZ+1.0, -1,0,0, textureX+16.0,textureY+16.0));
		renderData.push_back(Vertex(absX    , y+1.0, absZ+1.0, -1,0,0, textureX+16.0,textureY));
	}
	if(getCube(x,y-1,z) == 0) { // bottom face
		textureX = (textureIndexes[cubeID][4] % 4)*16;
		textureY = (textureIndexes[cubeID][4] / 4)*16;
		renderData.push_back(Vertex(absX+1.0, y    , absZ+1.0, 0,-1,0, textureX     ,textureY));
		renderData.push_back(Vertex(absX    , y    , absZ+1.0, 0,-1,0, textureX     ,textureY+16.0));
		renderData.push_back(Vertex(absX    , y    , absZ    , 0,-1,0, textureX+16.0,textureY+16.0));
		renderData.push_back(Vertex(absX+1.0, y    , absZ    , 0,-1,0, textureX+16.0,textureY));
	}
	if(getCube(x,y+1,z) == 0) { // top face
		textureX = (textureIndexes[cubeID][5] % 4)*16;
		textureY = (textureIndexes[cubeID][5] / 4)*16;
		renderData.push_back(Vertex(absX    , y+1.0, absZ+1.0, 0,1,0, textureX     ,textureY));
		renderData.push_back(Vertex(absX+1.0, y+1.0, absZ+1.0, 0,1,0, textureX     ,textureY+16.0));
		renderData.push_back(Vertex(absX+1.0, y+1.0, absZ    , 0,1,0, textureX+16.0,textureY+16.0));
		renderData.push_back(Vertex(absX    , y+1.0, absZ    , 0,1,0, textureX+16.0,textureY));
	}
}

void Chunk::draw() const {
	if(!outOfView) {
		glPushMatrix();
		glBindBuffer(GL_ARRAY_BUFFER, VBOID);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
		glNormalPointer(GL_FLOAT, sizeof(Vertex),(GLvoid*)(3*sizeof(float)));
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid*)(6*sizeof(float)));

		glDrawArrays(GL_QUADS, 0, renderData.size());

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glPopMatrix();
	}
}

void Chunk::makeVbo() {
	glGenBuffers(1, (GLuint*) &VBOID);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glBufferData(GL_ARRAY_BUFFER, renderData.size()*sizeof(Vertex), &renderData[0], GL_STATIC_DRAW);
}

bool Chunk::checkCulling(const Camera& cam) {
	sf::Vector2f point(XPOS*CHUNKWIDTH+CHUNKWIDTH/2, ZPOS*CHUNKWIDTH+CHUNKWIDTH/2);
	sf::Vector2f dir(-sin(-cam.rot.y*DEG_TO_RAD), -cos(-cam.rot.y*DEG_TO_RAD));
	sf::Vector2f pos(cam.pos.x,cam.pos.z);

	float distance = (dir.x*point.x + dir.y*point.y - dir.x*pos.x - dir.y*pos.y);
	return distance < -CHUNKWIDTH;
}

void Chunk::updateGrass(float deltaTime) { //only to be called by main update()
	if (grassTimer >= 0.01) {
		grassTimer -= 0.01;
		int x = rand()%CHUNKWIDTH;
		int y = rand()%CHUNKHEIGHT;
		int z = rand()%CHUNKWIDTH;
		if (getCube(x,y+1,z) != 0 && getCube(x,y,z) == 3) {
			setCube(x,y,z,1);
			markedForRedraw = true;
		}
	}
	grassTimer += deltaTime;
}

const int Chunk::textureIndexes[4][6] = { //order is front, back, left, right, bottom, top
										  {0,0,0,0,0,0}, //0 = air (empty, will never be used)
										  {2,2,2,2,2,2}, //1 = dirt
										  {3,3,3,3,3,3}, //2 = stone
										  {0,0,0,0,2,1} //3 = grass
										};
