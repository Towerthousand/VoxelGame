#ifndef MODEL_H
#define MODEL_H
#include "tools.hpp"

struct ModelCubeFileFormat;
struct ModelCube { //has no texture
		ModelCube (bool isAir, vec3f color);
		ModelCube (const ModelCubeFileFormat& c);
		bool isAir;
		vec3f color;
};

struct ModelCubeFileFormat { //has no texture
		ModelCubeFileFormat(const ModelCube &c);
		unsigned char r, g, b;
		bool isAir;
};

struct ModelVertex { //has no texture
		ModelVertex(float vx = 0.0, float vy = 0.0, float vz = 0.0,
			   float nx = 0.0, float ny = 0.0, float nz = 0.0,
			   float cr = 1.0, float cg = 1.0, float cb = 1.0, float ca = 1.0) :
			vx(vx), vy(vy), vz(vz),
			nx(nx), ny(ny), nz(nz),
			cr(cr), cg(cg), cb(cb), ca(ca)
		{}
		float vx,vy,vz,
		nx,ny,nz,
		cr,cg,cb,ca;
};

class Model {
	public:
		Model();
		~Model();

		bool loadVoxelization(std::string filePath);
		void draw(vec3f pos, vec3f rot, vec3f scale);
	private:
		ModelCube getCube(int x, int y, int z);
		void pushCubeToArray(int x, int y, int z);
		void makeVbo();

		int VBOID;
		int modelWidth; //in model cubes
		int modelHeight;//in model cubes
		int modelDepth; //in model cubes
		std::vector<std::vector<std::vector<ModelCube> > > cubes;
		std::vector<ModelVertex> renderData;
};

#endif // MODEL_H
