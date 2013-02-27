#include "Chunk.hpp"
#include "World.hpp"
#include <libsnoise/noise.h>
#include "noiseutils.h"

Chunk::Chunk(const int& x, const int& z, const int& seed, World &world) : XPOS(x), ZPOS(z), SEED(seed), parentWorld(world){
	cubes = std::vector<std::vector<std::vector<int> > >
			(CHUNKWIDTH,std::vector<std::vector<int> >
			(CHUNKHEIGHT,std::vector<int>
			(CHUNKWIDTH,0)));
	VBOID = XPOS*WORLDSIZE+ZPOS+1;
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

void Chunk::setCube(const int& x, const int& y, const int& z, const int &id) {
	if (y < 0 || y >= CHUNKHEIGHT) //over the skylimit, under bedrock
		return;
	else if (x >= CHUNKWIDTH || z >= CHUNKWIDTH || x < 0 || z < 0)//outside of this chunk
		parentWorld.setCubeAbs(x+(XPOS*CHUNKWIDTH),y,z+(ZPOS*CHUNKWIDTH),id);
	else //inside this chunk
		cubes[x][y][z] = id;
	markedForRedraw = true;
}

int Chunk::getCube(const int& x, const int& y, const int& z) const {
	if (y < 0 || y >= CHUNKHEIGHT) //over the skylimit, under bedrock
		return 0;
	if (x >= CHUNKWIDTH || z >= CHUNKWIDTH || x < 0 || z < 0)//outside of this chunk
		return parentWorld.getCubeAbs(x+(XPOS*CHUNKWIDTH),y,z+(ZPOS*CHUNKWIDTH));
	return cubes[x][y][z]; //inside current chunk
}

void Chunk::pushNormal(const int &x, const int &y, const int &z) {
	for (int i = 0; i < 4; ++i) {
		normals.push_back(sf::Vector3f( x, y, z));
	}
}

void Chunk::pushTexture(const int &textureID) {
	int x = (textureID % 4)*16; // 4 = number of textures/row, 16 = width
	int y = (textureID / 4)*16; // 4 = number of textures/row, 16 = height
	textureCoords.push_back(sf::Vector2f(x,y));
	textureCoords.push_back(sf::Vector2f(x,y+16.0));
	textureCoords.push_back(sf::Vector2f(x+16.0,y+16.0));
	textureCoords.push_back(sf::Vector2f(x+16.0,y));
}

void Chunk::update(const float& deltaTime) {
	updateGrass(deltaTime);
	if (markedForRedraw) { //empty arrays and re-do them
		markedForRedraw = false;
		vertexPoints.resize(0);
		normals.resize(0);
		textureCoords.resize(0);
		for(int z = 0; z < CHUNKWIDTH; ++z) {
			for(int y = 0; y < CHUNKHEIGHT; ++y) {
				for(int x = 0; x < CHUNKWIDTH; ++x) {
					if (getCube(x,y,z) != 0) { // only draw if it's not air
						if(getCube(x,y,z+1) == 0) { // front face
							pushNormal(0,0,1);
							pushTexture(textureIndexes[getCube(x,y,z)][0]);
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)    , y+1.0, (z+CHUNKWIDTH*ZPOS)+1.0));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)    , y    , (z+CHUNKWIDTH*ZPOS)+1.0));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)+1.0, y    , (z+CHUNKWIDTH*ZPOS)+1.0));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)+1.0, y+1.0, (z+CHUNKWIDTH*ZPOS)+1.0));
						}
						if(getCube(x,y,z-1) == 0) { // back face
							pushNormal(0,0,-1);
							pushTexture(textureIndexes[getCube(x,y,z)][1]);
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)+1.0, y+1.0, (z+CHUNKWIDTH*ZPOS)));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)+1.0, y    , (z+CHUNKWIDTH*ZPOS)));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)    , y    , (z+CHUNKWIDTH*ZPOS)));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)    , y+1.0, (z+CHUNKWIDTH*ZPOS)));
						}
						if(getCube(x+1,y,z) == 0) { // left face
							pushNormal(1,0,0);
							pushTexture(textureIndexes[getCube(x,y,z)][2]);
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)+1.0, y+1.0, (z+CHUNKWIDTH*ZPOS)+1.0));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)+1.0, y    , (z+CHUNKWIDTH*ZPOS)+1.0));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)+1.0, y    , (z+CHUNKWIDTH*ZPOS)    ));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)+1.0, y+1.0, (z+CHUNKWIDTH*ZPOS)    ));
						}
						if(getCube(x-1,y,z) == 0) { // right face
							pushNormal(-1,0,0);
							pushTexture(textureIndexes[getCube(x,y,z)][3]);
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)    , y+1.0, (z+CHUNKWIDTH*ZPOS)    ));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)    , y    , (z+CHUNKWIDTH*ZPOS)    ));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)    , y    , (z+CHUNKWIDTH*ZPOS)+1.0));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)    , y+1.0, (z+CHUNKWIDTH*ZPOS)+1.0));
						}
						if(getCube(x,y-1,z) == 0) { // bottom face
							pushNormal(0,-1, 0);
							pushTexture(textureIndexes[getCube(x,y,z)][4]);
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)+1.0, y    , (z+CHUNKWIDTH*ZPOS)+1.0));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)    , y    , (z+CHUNKWIDTH*ZPOS)+1.0));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)    , y    , (z+CHUNKWIDTH*ZPOS)    ));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)+1.0, y    , (z+CHUNKWIDTH*ZPOS)    ));
						}
						if(getCube(x,y+1,z) == 0) { // top face
							pushNormal(0,1,0);
							pushTexture(textureIndexes[getCube(x,y,z)][5]);
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)    , y+1.0, (z+CHUNKWIDTH*ZPOS)+1.0));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)+1.0, y+1.0, (z+CHUNKWIDTH*ZPOS)+1.0));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)+1.0, y+1.0, (z+CHUNKWIDTH*ZPOS)    ));
							vertexPoints.push_back(sf::Vector3f((x+CHUNKWIDTH*XPOS)    , y+1.0, (z+CHUNKWIDTH*ZPOS)    ));
						}
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

	glVertexPointer(3, GL_FLOAT, 0, 0);
	glNormalPointer(GL_FLOAT, 0, (GLvoid*)(vertexPoints.size()*sizeof(float)*3));
	glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)(vertexPoints.size()*sizeof(float)*3+normals.size()*sizeof(float)*3));

	glDrawArrays(GL_QUADS, 0, vertexPoints.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glPopMatrix();
}

void Chunk::makeVbo() {
	uint nvertex = vertexPoints.size()*sizeof(float)*3;
	uint nnormals = normals.size()*sizeof(float)*3;
	uint ntexture = textureCoords.size()*sizeof(float)*2;
	glGenBuffers(1, (GLuint*) &VBOID);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glBufferData(GL_ARRAY_BUFFER, nvertex+nnormals+ntexture, 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nvertex, &vertexPoints[0]);
	glBufferSubData(GL_ARRAY_BUFFER, nvertex, nnormals, &normals[0]);
	glBufferSubData(GL_ARRAY_BUFFER, nvertex+nnormals, ntexture, &textureCoords[0]);
}

void Chunk::updateGrass(const float& deltaTime) { //only to be called by main update()
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
	{0,0,0,0,2,1}  //3 = grass
};
