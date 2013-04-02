#include "Model.hpp"

ModelCube::ModelCube (bool isAir, vec3f color) : isAir(isAir), color(color) {}
ModelCube::ModelCube (const ModelCubeFileFormat& c) : isAir(c.isAir) , color(c.r/255.0,c.g/255.0,c.b/255.0){}
ModelCubeFileFormat::ModelCubeFileFormat(const ModelCube &c): r(c.color.x*255), g(c.color.y*255), b(c.color.z*255), isAir(c.isAir){}

Model::Model() {
}

Model::~Model() {
}

bool Model::loadVoxelization(std::string filePath) {
	//Load file as matrix of cubes
	std::ifstream file;
	file.open(filePath.c_str());
	if(!file) {
		outLog("#ERROR Could not load voxelization \"" + filePath + "\"");
		return false;
	}
	file.read((char *) &modelWidth, sizeof(int));
	file.read((char *) &modelHeight, sizeof(int));
	file.read((char *) &modelWidth, sizeof(int));
	cubes = std::vector<std::vector<std::vector<ModelCube> > >(modelWidth,std::vector<std::vector<ModelCube> >
															   (modelHeight,std::vector<ModelCube>
																(modelWidth,ModelCube(false,vec3f(0,0,0)))));
	ModelCubeFileFormat c(ModelCube(false,vec3f(0,0,0)));
	for(int x = 0; x < modelWidth; ++x) {
		for(int y = 0; y < modelHeight; ++y) {
			for(int z = 0; z < modelWidth; ++z) {
				file.read((char *) &c,sizeof(ModelCubeFileFormat));
				cubes[x][y][z] = ModelCube(c);
			}
		}
	}

	//Push back the vertices
	for(int x = 0; x < modelWidth; ++x) {
		for(int y = 0; y < modelHeight; ++y) {
			for(int z = 0; z < modelWidth; ++z) {
				pushCubeToArray(x,y,z);
			}
		}
	}
	//Make the VBO with the render data
	makeVbo();
	return true;
}

ModelCube Model::getCube(int x, int y, int z) {
	if (x >= modelWidth || x < 0
		|| y >= modelHeight || y < 0
		|| z >= modelWidth || z< 0)
		return ModelCube(true,vec3f(0,0,0));
	return cubes[x][y][z];
}

void Model::pushCubeToArray(int x,int y, int z) { //I DON'T KNOW HOW TO MAKE THIS COMPACT
	float lindA = 1.0,lindB = 1.0,lindC = 1.0,lindD = 1.0, lindE = 1.0;
	//STRUCTURE PER VERTEX: Vx,Vy,Vz,
	//						Nx,Ny,Nz,
	//						Cr,Cg,Cb,Ca
	vec3f color = getCube(x,y,z).color;
	if(getCube(x,y,z+1).isAir) { // front face
		lindA = (getCube(x,y,z+1).isAir + getCube(x,y+1,z+1).isAir +
				 getCube(x-1,y,z+1).isAir + getCube(x-1,y+1,z+1).isAir)/4.0;
		lindB = (getCube(x,y,z+1).isAir + getCube(x,y-1,z+1).isAir +
				 getCube(x-1,y,z+1).isAir + getCube(x-1,y-1,z+1).isAir)/4.0;
		lindC = (getCube(x,y,z+1).isAir + getCube(x,y-1,z+1).isAir +
				 getCube(x+1,y,z+1).isAir + getCube(x+1,y-1,z+1).isAir)/4.0;
		lindD = (getCube(x,y,z+1).isAir + getCube(x,y+1,z+1).isAir +
				 getCube(x+1,y,z+1).isAir + getCube(x+1,y+1,z+1).isAir)/4.0;
		lindE = (lindA+lindB+lindC+lindD)/4.0;
		//t1
		renderData.push_back(ModelVertex(x+1.0, y+1.0, z+1.0, 0,0,1, lindD*color.x,lindD*color.y,lindD*color.z,1.0));
		renderData.push_back(ModelVertex(x    , y+1.0, z+1.0, 0,0,1, lindA*color.x,lindA*color.y,lindA*color.z,1.0));
		renderData.push_back(ModelVertex(x+0.5, y+0.5, z+1.0, 0,0,1, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t2
		renderData.push_back(ModelVertex(x+1.0, y    , z+1.0, 0,0,1, lindC*color.x,lindC*color.y,lindC*color.z,1.0));
		renderData.push_back(ModelVertex(x+1.0, y+1.0, z+1.0, 0,0,1, lindD*color.x,lindD*color.y,lindD*color.z,1.0));
		renderData.push_back(ModelVertex(x+0.5, y+0.5, z+1.0, 0,0,1, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t3
		renderData.push_back(ModelVertex(x    , y    , z+1.0, 0,0,1, lindB*color.x,lindB*color.y,lindB*color.z,1.0));
		renderData.push_back(ModelVertex(x+1.0, y    , z+1.0, 0,0,1, lindC*color.x,lindC*color.y,lindC*color.z,1.0));
		renderData.push_back(ModelVertex(x+0.5, y+0.5, z+1.0, 0,0,1, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t4
		renderData.push_back(ModelVertex(x    , y+1.0, z+1.0, 0,0,1, lindA*color.x,lindA*color.y,lindA*color.z,1.0));
		renderData.push_back(ModelVertex(x    , y    , z+1.0, 0,0,1, lindB*color.x,lindB*color.y,lindB*color.z,1.0));
		renderData.push_back(ModelVertex(x+0.5, y+0.5, z+1.0, 0,0,1, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
	}
	if(getCube(x,y,z-1).isAir) { // back face
		lindA = (getCube(x,y,z-1).isAir + getCube(x,y+1,z-1).isAir +
				 getCube(x+1,y,z-1).isAir + getCube(x+1,y+1,z-1).isAir)/4.0;
		lindB = (getCube(x,y,z-1).isAir + getCube(x,y-1,z-1).isAir +
				 getCube(x+1,y,z-1).isAir + getCube(x+1,y-1,z-1).isAir)/4.0;
		lindC = (getCube(x,y,z-1).isAir + getCube(x,y-1,z-1).isAir +
				 getCube(x-1,y,z-1).isAir + getCube(x-1,y-1,z-1).isAir)/4.0;
		lindD = (getCube(x,y,z-1).isAir + getCube(x,y+1,z-1).isAir +
				 getCube(x-1,y,z-1).isAir + getCube(x-1,y+1,z-1).isAir)/4.0;
		lindE = (lindA+lindB+lindC+lindD)/4.0;
		//t1
		renderData.push_back(ModelVertex(x    , y+1.0, z    , 0,0,-1, lindD*color.x,lindD*color.y,lindD*color.z,1.0));
		renderData.push_back(ModelVertex(x+1.0, y+1.0, z    , 0,0,-1, lindA*color.x,lindA*color.y,lindA*color.z,1.0));
		renderData.push_back(ModelVertex(x+0.5, y+0.5, z    , 0,0,-1, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t2
		renderData.push_back(ModelVertex(x+1.0, y+1.0, z    , 0,0,-1, lindA*color.x,lindA*color.y,lindA*color.z,1.0));
		renderData.push_back(ModelVertex(x+1.0, y    , z    , 0,0,-1, lindB*color.x,lindB*color.y,lindB*color.z,1.0));
		renderData.push_back(ModelVertex(x+0.5, y+0.5, z    , 0,0,-1, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t3
		renderData.push_back(ModelVertex(x+1.0, y    , z    , 0,0,-1, lindB*color.x,lindB*color.y,lindB*color.z,1.0));
		renderData.push_back(ModelVertex(x    , y    , z    , 0,0,-1, lindC*color.x,lindC*color.y,lindC*color.z,1.0));
		renderData.push_back(ModelVertex(x+0.5, y+0.5, z    , 0,0,-1, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t4
		renderData.push_back(ModelVertex(x    , y    , z    , 0,0,-1, lindC*color.x,lindC*color.y,lindC*color.z,1.0));
		renderData.push_back(ModelVertex(x    , y+1.0, z    , 0,0,-1, lindD*color.x,lindD*color.y,lindD*color.z,1.0));
		renderData.push_back(ModelVertex(x+0.5, y+0.5, z    , 0,0,-1, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
	}
	if(getCube(x+1,y,z).isAir) { // left face
		lindA = (getCube(x+1,y,z).isAir + getCube(x+1,y+1,z).isAir +
				 getCube(x+1,y,z+1).isAir + getCube(x+1,y+1,z+1).isAir)/4.0;
		lindB = (getCube(x+1,y,z).isAir + getCube(x+1,y-1,z).isAir +
				 getCube(x+1,y,z+1).isAir + getCube(x+1,y-1,z+1).isAir)/4.0;
		lindC = (getCube(x+1,y,z).isAir + getCube(x+1,y-1,z).isAir +
				 getCube(x+1,y,z-1).isAir + getCube(x+1,y-1,z-1).isAir)/4.0;
		lindD = (getCube(x+1,y,z).isAir + getCube(x+1,y+1,z).isAir +
				 getCube(x+1,y,z-1).isAir + getCube(x+1,y+1,z-1).isAir)/4.0;
		lindE = (lindA+lindB+lindC+lindD)/4.0;
		//t1
		renderData.push_back(ModelVertex(x+1.0, y+1.0, z    , 1,0,0, lindD*color.x,lindD*color.y,lindD*color.z,1.0));
		renderData.push_back(ModelVertex(x+1.0, y+1.0, z+1.0, 1,0,0, lindA*color.x,lindA*color.y,lindA*color.z,1.0));
		renderData.push_back(ModelVertex(x+1.0, y+0.5, z+0.5, 1,0,0, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t2
		renderData.push_back(ModelVertex(x+1.0, y    , z    , 1,0,0, lindC*color.x,lindC*color.y,lindC*color.z,1.0));
		renderData.push_back(ModelVertex(x+1.0, y+1.0, z    , 1,0,0, lindD*color.x,lindD*color.y,lindD*color.z,1.0));
		renderData.push_back(ModelVertex(x+1.0, y+0.5, z+0.5, 1,0,0, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t3
		renderData.push_back(ModelVertex(x+1.0, y    , z+1.0, 1,0,0, lindB*color.x,lindB*color.y,lindB*color.z,1.0));
		renderData.push_back(ModelVertex(x+1.0, y    , z    , 1,0,0, lindC*color.x,lindC*color.y,lindC*color.z,1.0));
		renderData.push_back(ModelVertex(x+1.0, y+0.5, z+0.5, 1,0,0, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t4
		renderData.push_back(ModelVertex(x+1.0, y+1.0, z+1.0, 1,0,0, lindA*color.x,lindA*color.y,lindA*color.z,1.0));
		renderData.push_back(ModelVertex(x+1.0, y    , z+1.0, 1,0,0, lindB*color.x,lindB*color.y,lindB*color.z,1.0));
		renderData.push_back(ModelVertex(x+1.0, y+0.5, z+0.5, 1,0,0, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
	}
	if(getCube(x-1,y,z).isAir) { // right face
		lindA = (getCube(x-1,y,z).isAir + getCube(x-1,y+1,z).isAir +
				 getCube(x-1,y,z-1).isAir + getCube(x-1,y+1,z-1).isAir)/4.0;
		lindB = (getCube(x-1,y,z).isAir + getCube(x-1,y-1,z).isAir +
				 getCube(x-1,y,z-1).isAir + getCube(x-1,y-1,z-1).isAir)/4.0;
		lindC = (getCube(x-1,y,z).isAir + getCube(x-1,y-1,z).isAir +
				 getCube(x-1,y,z+1).isAir + getCube(x-1,y-1,z+1).isAir)/4.0;
		lindD = (getCube(x-1,y,z).isAir + getCube(x-1,y+1,z).isAir +
				 getCube(x-1,y,z+1).isAir + getCube(x-1,y+1,z+1).isAir)/4.0;
		lindE = (lindA+lindB+lindC+lindD)/4.0;
		//t1
		renderData.push_back(ModelVertex(x    , y+1.0, z+1.0, -1,0,0, lindD*color.x,lindD*color.y,lindD*color.z,1.0));
		renderData.push_back(ModelVertex(x    , y+1.0, z    , -1,0,0, lindA*color.x,lindA*color.y,lindA*color.z,1.0));
		renderData.push_back(ModelVertex(x    , y+0.5, z+0.5, -1,0,0, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t2
		renderData.push_back(ModelVertex(x    , y+1.0, z    , -1,0,0, lindA*color.x,lindA*color.y,lindA*color.z,1.0));
		renderData.push_back(ModelVertex(x    , y    , z    , -1,0,0, lindB*color.x,lindB*color.y,lindB*color.z,1.0));
		renderData.push_back(ModelVertex(x    , y+0.5, z+0.5, -1,0,0, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t3
		renderData.push_back(ModelVertex(x    , y    , z    , -1,0,0, lindB*color.x,lindB*color.y,lindB*color.z,1.0));
		renderData.push_back(ModelVertex(x    , y    , z+1.0, -1,0,0, lindC*color.x,lindC*color.y,lindC*color.z,1.0));
		renderData.push_back(ModelVertex(x    , y+0.5, z+0.5, -1,0,0, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t4
		renderData.push_back(ModelVertex(x    , y    , z+1.0, -1,0,0, lindC*color.x,lindC*color.y,lindC*color.z,1.0));
		renderData.push_back(ModelVertex(x    , y+1.0, z+1.0, -1,0,0, lindD*color.x,lindD*color.y,lindD*color.z,1.0));
		renderData.push_back(ModelVertex(x    , y+0.5, z+0.5, -1,0,0, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
	}
	if(getCube(x,y-1,z).isAir) { // bottom face
		lindA = (getCube(x,y-1,z).isAir + getCube(x+1,y-1,z).isAir +
				 getCube(x,y-1,z+1).isAir + getCube(x+1,y-1,z+1).isAir)/4.0;
		lindB = (getCube(x,y-1,z).isAir + getCube(x-1,y-1,z).isAir +
				 getCube(x,y-1,z+1).isAir + getCube(x-1,y-1,z+1).isAir)/4.0;
		lindC = (getCube(x,y-1,z).isAir + getCube(x-1,y-1,z).isAir +
				 getCube(x,y-1,z-1).isAir + getCube(x-1,y-1,z-1).isAir)/4.0;
		lindD = (getCube(x,y-1,z).isAir + getCube(x+1,y-1,z).isAir +
				 getCube(x,y-1,z-1).isAir + getCube(x+1,y-1,z-1).isAir)/4.0;
		lindE = (lindA+lindB+lindC+lindD)/4.0;
		//t1
		renderData.push_back(ModelVertex(x    , y    , z    , 0,-1,0, lindC*color.x,lindC*color.y,lindC*color.z,1.0));
		renderData.push_back(ModelVertex(x+1.0, y    , z    , 0,-1,0, lindD*color.x,lindD*color.y,lindD*color.z,1.0));
		renderData.push_back(ModelVertex(x+0.5, y    , z+0.5, 0,-1,0, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t2
		renderData.push_back(ModelVertex(x+1.0, y    , z    , 0,-1,0, lindD*color.x,lindD*color.y,lindD*color.z,1.0));
		renderData.push_back(ModelVertex(x+1.0, y    , z+1.0, 0,-1,0, lindA*color.x,lindA*color.y,lindA*color.z,1.0));
		renderData.push_back(ModelVertex(x+0.5, y    , z+0.5, 0,-1,0, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t3
		renderData.push_back(ModelVertex(x+1.0, y    , z+1.0, 0,-1,0, lindA*color.x,lindA*color.y,lindA*color.z,1.0));
		renderData.push_back(ModelVertex(x    , y    , z+1.0, 0,-1,0, lindB*color.x,lindB*color.y,lindB*color.z,1.0));
		renderData.push_back(ModelVertex(x+0.5, y    , z+0.5, 0,-1,0, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t4
		renderData.push_back(ModelVertex(x    , y    , z+1.0, 0,-1,0, lindB*color.x,lindB*color.y,lindB*color.z,1.0));
		renderData.push_back(ModelVertex(x    , y    , z    , 0,-1,0, lindC*color.x,lindC*color.y,lindC*color.z,1.0));
		renderData.push_back(ModelVertex(x+0.5, y    , z+0.5, 0,-1,0, lindE*color.x,lindE*color.y,lindE*color.z,1.0));
	}
	if(getCube(x,y+1,z).isAir) { // top face
		lindA = (getCube(x,y+1,z).isAir + getCube(x-1,y+1,z).isAir +
				 getCube(x,y+1,z+1).isAir + getCube(x-1,y+1,z+1).isAir)/4.0;
		lindB = (getCube(x,y+1,z).isAir + getCube(x+1,y+1,z).isAir +
				 getCube(x,y+1,z+1).isAir + getCube(x+1,y+1,z+1).isAir)/4.0;
		lindC = (getCube(x,y+1,z).isAir + getCube(x+1,y+1,z).isAir +
				 getCube(x,y+1,z-1).isAir + getCube(x+1,y+1,z-1).isAir)/4.0;
		lindD = (getCube(x,y+1,z).isAir + getCube(x-1,y+1,z).isAir +
				 getCube(x,y+1,z-1).isAir + getCube(x-1,y+1,z-1).isAir)/4.0;
		lindE = (lindA+lindB+lindC+lindD)/4.0;
		//t1
		renderData.push_back(ModelVertex(x+1.0, y+1.0, z    , 0,1,0 , lindC*color.x,lindC*color.y,lindC*color.z,1.0));
		renderData.push_back(ModelVertex(x    , y+1.0, z    , 0,1,0 , lindD*color.x,lindD*color.y,lindD*color.z,1.0));
		renderData.push_back(ModelVertex(x+0.5, y+1.0, z+0.5, 0,1,0 , lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t2
		renderData.push_back(ModelVertex(x    , y+1.0, z    , 0,1,0 , lindD*color.x,lindD*color.y,lindD*color.z,1.0));
		renderData.push_back(ModelVertex(x    , y+1.0, z+1.0, 0,1,0 , lindA*color.x,lindA*color.y,lindA*color.z,1.0));
		renderData.push_back(ModelVertex(x+0.5, y+1.0, z+0.5, 0,1,0 , lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t3
		renderData.push_back(ModelVertex(x    , y+1.0, z+1.0, 0,1,0 , lindA*color.x,lindA*color.y,lindA*color.z,1.0));
		renderData.push_back(ModelVertex(x+1.0, y+1.0, z+1.0, 0,1,0 , lindB*color.x,lindB*color.y,lindB*color.z,1.0));
		renderData.push_back(ModelVertex(x+0.5, y+1.0, z+0.5, 0,1,0 , lindE*color.x,lindE*color.y,lindE*color.z,1.0));
		//t4
		renderData.push_back(ModelVertex(x+1.0, y+1.0, z+1.0, 0,1,0 , lindB*color.x,lindB*color.y,lindB*color.z,1.0));
		renderData.push_back(ModelVertex(x+1.0, y+1.0, z    , 0,1,0 , lindC*color.x,lindC*color.y,lindC*color.z,1.0));
		renderData.push_back(ModelVertex(x+0.5, y+1.0, z+0.5, 0,1,0 , lindE*color.x,lindE*color.y,lindE*color.z,1.0));
	}
}


void Model::makeVbo() {
	glGenBuffers(1, (GLuint*) &VBOID);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glBufferData(GL_ARRAY_BUFFER, renderData.size()*sizeof(ModelVertex), &renderData[0], GL_STATIC_DRAW);
}
