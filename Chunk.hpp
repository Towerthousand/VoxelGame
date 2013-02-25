#ifndef CHUNK_HPP
#define CHUNK_HPP
#include "tools.hpp"

class World;
class Chunk { //16*16*128
	public:
		Chunk(const int& x, const int& z, const int& seed, World &world);
		~Chunk();

		void populate();

		void pushNormal(const int& x,const int& y,const int& z);
		void pushTexture(const int& textureID);
		void draw() const;
		void makeVbo();

		void update(const float& deltaTime);

		void setCube(const int& x, const int& y, const int& z, const int&id);
		int getCube(const int& x, const int& y, const int& z) const;

		void updateGrass(const float& deltaTime);

		std::vector<std::vector<std::vector<int> > > cubes;
		std::vector<sf::Vector3f> vertexPoints;
		std::vector<sf::Vector3f> normals;
		std::vector<sf::Vector2f> textureCoords;
		static const int textureIndexes[4][6];

		int XPOS; //x pos of chunk inside world matrix
		int ZPOS; //z pos of chunk inside world matrix
		int SEED; //constant across chunks
		uint VBOID;

		bool markedForRedraw;
		float grassTimer;

		World& parentWorld;
};

#endif // WORLD_HPP
