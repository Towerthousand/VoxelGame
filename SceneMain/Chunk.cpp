#include "Chunk.hpp"
#include "World.hpp"
#include <libnoise/noise.h>
#include "noiseutils.h"

Chunk::Chunk(int x, int z, int seed, World &world) : XPOS(x), ZPOS(z), SEED(seed), parentWorld(world){
	cubes = std::vector<std::vector<std::vector<Cube> > >
			(CHUNKWIDTH,std::vector<std::vector<Cube> >
			 (CHUNKHEIGHT,std::vector<Cube>
			  (CHUNKWIDTH,Cube(0,1))));
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
				cubes[x][y][z].id = 2;
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
					cubes[x][y+SEALEVEL][z].id = 2;
				else if (y <= height-1 && y+SEALEVEL < CHUNKHEIGHT)
					cubes[x][y+SEALEVEL][z].id = 1;
				else if(y == height && y+SEALEVEL < CHUNKHEIGHT)
					cubes[x][y+SEALEVEL][z].id = 3;
			}
		}
	}
	markedForRedraw = true;
}

void Chunk::updateCube(int x, int y, int z) {
	if (y < 0 || y >= CHUNKHEIGHT) //over the skylimit, under bedrock
		return;
	else if (x >= CHUNKWIDTH-1 || z >= CHUNKWIDTH-1 || x <= 0 || z <= 0)//outside of this chunk or on border
		parentWorld.updateCubeAbs(x+(XPOS*CHUNKWIDTH),y,z+(ZPOS*CHUNKWIDTH));
	else
		markedForRedraw = true;
}

Cube &Chunk::getCube(int x, int y, int z) {
	if (y < 0 || y >= CHUNKHEIGHT) //over the skylimit, under bedrock
		return World::empty;
	if (x >= CHUNKWIDTH || z >= CHUNKWIDTH || x < 0 || z < 0)//outside of this chunk
		return parentWorld.getCubeAbs(x+(XPOS*CHUNKWIDTH),y,z+(ZPOS*CHUNKWIDTH));
	return cubes[x][y][z]; //inside current chunk
}

void Chunk::calculateLight() {
	//BFS TO THE MAX, bug: I don't know how to preserve light across chunks, since every chunk only looks
	//					   for its own light blocks
	std::queue<sf::Vector3i> blocksToCheck;
	for(int z = 0; z < CHUNKWIDTH; ++z) {
		for(int y = 0; y < CHUNKHEIGHT; ++y) {
			for(int x = 0; x < CHUNKWIDTH; ++x) {
				if (getCube(x,y,z).id == 4) { //light block
					cubes[x][y][z].light = LIGHTMAX;
					blocksToCheck.push(sf::Vector3i(x,y,z));
				}
				else if (getCube(x,y,z).id == 0){
					getCube(x,y,z).light = 1;
				}
				else
					getCube(x,y,z).light = 0;
			}
		}
	}
	while(!blocksToCheck.empty()) {
		sf::Vector3i source = blocksToCheck.front();
		if(getCube(source.x,source.y,source.z).light == 2) {
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

void Chunk::processCubeLighting(sf::Vector3i source, sf::Vector3i offset, std::queue<sf::Vector3i> &queue) {
	sf::Vector3i subject = source+offset;
	if(getCube(subject.x,subject.y,subject.z).id == 0) {
		if(getCube(subject.x,subject.y,subject.z).light == getCube(source.x,source.y,source.z).light-1) {
			queue.push(subject);
		}
		else if(getCube(subject.x,subject.y,subject.z).light < getCube(source.x,source.y,source.z).light-1) {
			queue.push(subject);
			getCube(subject.x,subject.y,subject.z).light = getCube(source.x,source.y,source.z).light-1;
		}
	}
}

void Chunk::update(float deltaTime) {
	updateGrass(deltaTime);
	if (markedForRedraw) { //empty arrays and re-do them
		markedForRedraw = false;
		calculateLight();
		renderData.resize(0);
		int cubeID;
		for(int z = 0; z < CHUNKWIDTH; ++z) {
			for(int y = 0; y < CHUNKHEIGHT; ++y) {
				for(int x = 0; x < CHUNKWIDTH; ++x) {
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

void Chunk::pushCubeToArray(int x,int y, int z,int cubeID) {
	int absX = x+CHUNKWIDTH*XPOS;
	int absZ = z+CHUNKWIDTH*ZPOS;
	int texY, texX;
	float lind;
	//STRUCTURE PER VERTEX: Vx,Vy,Vz,
	//						Nx,Ny,Nz,
	//						Tx,Ty,
	//						Cr,Cg,Cb,Ca
	if(getCube(x,y,z+1).id == 0) { // front face
		lind = getCube(x,y,z+1).light/LIGHTMAX; //light index
		texX = (textureIndexes[cubeID][0] % 32)*16; // 4 = number of textures/row, 16 = width
		texY = (textureIndexes[cubeID][0] / 32)*16; // 4 = number of textures/row, 16 = height
		renderData.push_back(Vertex(absX    , y+1.0, absZ+1.0, 0,0,1 , texX     ,texY     , lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , y    , absZ+1.0, 0,0,1 , texX	    ,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, y    , absZ+1.0, 0,0,1 , texX+16.0,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, y+1.0, absZ+1.0, 0,0,1 , texX+16.0,texY     , lind,lind,lind,1.0));
	}
	if(getCube(x,y,z-1).id == 0) { // back face
		lind = getCube(x,y,z-1).light/LIGHTMAX;
		texX = (textureIndexes[cubeID][1] % 32)*16;
		texY = (textureIndexes[cubeID][1] / 32)*16;
		renderData.push_back(Vertex(absX+1.0, y+1.0, absZ    , 0,0,-1, texX     ,texY     , lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, y    , absZ    , 0,0,-1, texX	    ,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , y    , absZ    , 0,0,-1, texX+16.0,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , y+1.0, absZ    , 0,0,-1, texX+16.0,texY     , lind,lind,lind,1.0));
	}
	if(getCube(x+1,y,z).id == 0) { // left face
		lind = getCube(x+1,y,z).light/LIGHTMAX;
		texX = (textureIndexes[cubeID][2] % 32)*16;
		texY = (textureIndexes[cubeID][2] / 32)*16;
		renderData.push_back(Vertex(absX+1.0, y+1.0, absZ+1.0, 1,0,0 , texX     ,texY     , lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, y    , absZ+1.0, 1,0,0 , texX	    ,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, y    , absZ    , 1,0,0 , texX+16.0,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, y+1.0, absZ    , 1,0,0 , texX+16.0,texY     , lind,lind,lind,1.0));
	}
	if(getCube(x-1,y,z).id == 0) { // right face
		lind = getCube(x-1,y,z).light/LIGHTMAX;
		texX = (textureIndexes[cubeID][3] % 32)*16;
		texY = (textureIndexes[cubeID][3] / 32)*16;
		renderData.push_back(Vertex(absX    , y+1.0, absZ    , -1,0,0, texX     ,texY     , lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , y    , absZ    , -1,0,0, texX	    ,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , y    , absZ+1.0, -1,0,0, texX+16.0,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , y+1.0, absZ+1.0, -1,0,0, texX+16.0,texY     , lind,lind,lind,1.0));
	}
	if(getCube(x,y-1,z).id == 0) { // bottom face
		lind = getCube(x,y-1,z).light/LIGHTMAX;
		texX = (textureIndexes[cubeID][4] % 32)*16;
		texY = (textureIndexes[cubeID][4] / 32)*16;
		renderData.push_back(Vertex(absX+1.0, y    , absZ+1.0, 0,-1,0, texX     ,texY     , lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , y    , absZ+1.0, 0,-1,0, texX     ,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , y    , absZ    , 0,-1,0, texX+16.0,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, y    , absZ    , 0,-1,0, texX+16.0,texY     , lind,lind,lind,1.0));
	}
	if(getCube(x,y+1,z).id == 0) { // top face
		lind = getCube(x,y+1,z).light/LIGHTMAX;
		texX = (textureIndexes[cubeID][5] % 32)*16;
		texY = (textureIndexes[cubeID][5] / 32)*16;
		renderData.push_back(Vertex(absX    , y+1.0, absZ+1.0, 0,1,0 , texX     ,texY     , lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, y+1.0, absZ+1.0, 0,1,0 , texX     ,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX+1.0, y+1.0, absZ    , 0,1,0 , texX+16.0,texY+16.0, lind,lind,lind,1.0));
		renderData.push_back(Vertex(absX    , y+1.0, absZ    , 0,1,0 , texX+16.0,texY     , lind,lind,lind,1.0));
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
		if (getCube(x,y+1,z).id != 0 && getCube(x,y,z).id == 3) {
			getCube(x,y,z).id = 1;
			updateCube(x,y,z);
			markedForRedraw = true;
		}
	}
	grassTimer += deltaTime;
}

const int Chunk::textureIndexes[5][6] = { //order is front, back, left, right, bottom, top
										  {0,0,0,0,0,0}, //0 = air (empty, will never be used)
										  {2,2,2,2,2,2}, //1 = dirt
										  {3,3,3,3,3,3}, //2 = stone
										  {0,0,0,0,2,1}, //3 = grass
										  {4,4,4,4,4,4}  //4 = lolwtf
										};
