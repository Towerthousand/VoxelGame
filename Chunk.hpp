#ifndef CHUNK_HPP
#define CHUNK_HPP
#include "tools.hpp"

class World;
class Chunk { //16*16*128
	public:
		Chunk(int x, int z, int seed, World &world);
		~Chunk();

		void populate();
		void draw() const;
		void update(float deltaTime);
		void setCube(int x, int y, int z, int id);
		int getCube(int x, int y, int z) const;
		void pushNormal(int x, int y, int z);
		void pushTexture(int textureID);
		void updateGrass(float deltaTime);

		std::vector<sf::Vector3f> vertexPoints;
		std::vector<sf::Vector3f> normals;
		std::vector<sf::Vector2f> textureCoords;
		static const int textureIndexes[4][6];
		bool markedForRedraw;
		int XPOS; //x pos of chunk inside world matrix
		int ZPOS; //z pos of chunk inside world matrix
		int SEED; //constant across chunks
		std::vector<std::vector<std::vector<int> > > cubes;
		float grassTimer;
		World& parentWorld;
};

#endif // WORLD_HPP
