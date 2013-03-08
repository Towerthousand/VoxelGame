#ifndef CHUNK_HPP
#define CHUNK_HPP
#include "tools.hpp"
#include "Camera.hpp"

class World;
class Chunk { //16*16*128
	public:
		Chunk(int x, int y, int z, World &world);
		~Chunk();

		enum FACE {
			FRONT = 0,
			BACK,
			LEFT,
			RIGHT,
			BOTTOM,
			TOP
		};

		void populate();
		void calculateLight();
        void processCubeLighting(sf::Vector3i source, sf::Vector3i offset, std::queue<sf::Vector3i> &queue);
		void update(float deltaTime);
		void pushCubeToArray(int x, int y, int z, int cubeID);
		void pushCubeFace(sf::Vector3f n, int x, int y, int z, int cubeID); //n = normal of the face to be pushed
		void draw() const;
		bool checkCulling(const Camera &cam);
		void makeVbo();

        bool outOfBounds(int x, int y, int z);
        Cube getCube(int x, int y, int z);

		void updateGrass(float deltaTime);

        std::vector<std::vector<std::vector<Cube> > > cubes;
		std::vector<Vertex> renderData;
		static const int textureIndexes[5][6];

		int XPOS; //x pos of chunk inside world matrix
		int ZPOS; //z pos of chunk inside world matrix
        int YPOS; //y pos of chunk inside world matrix
		int SEED; //constant across chunks
		uint VBOID;
		bool markedForRedraw;
		bool outOfView;
		float grassTimer;

		World& parentWorld;
};

#endif // CHUNK_HPP
