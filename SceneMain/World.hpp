#ifndef WORLD_HPP
#define WORLD_HPP
#include "tools.hpp"


//chunk grid coords:
//0 <= x < WORLDWIDTH*CHUNKSIZE
//0 <= z < WORLDWIDTH*CHUNKSIZE
//0 <= y < WORLDHEIGHT*CHUNKSIZE
//
//infdev coordinates (Work In Progress):
//MIN_INT <= x,z <= MAX_INT
//0 <= y < WORLDHEIGHT*CHUNKSIZE

class Chunk;
class Camera;
class World {
public:
    World();
    ~World();
    bool outOfBounds(int x, int y, int z);
    Cube getCubeAbs(int x, int y, int z) const;
    void setCubeAbs(int x, int y, int z, Cube c);
    void regenChunk(int x, int y, int z, int seed);
    void draw() const;
    void drawWireCube(const sf::Vector3f& pos) const;
    void update(float deltaTime, const Camera &camera);
    void traceView(const Camera& player, float tMax);

    bool playerTargetsBlock;
    int chunksDrawn;
    sf::Vector3f targetedBlock;
    sf::Vector3f last;
    static const Cube empty;
    static const int vertexPoints[8][3];
    static const int indexes[24];
    std::vector<std::vector<std::vector<Chunk*> > > chunks;
};

#endif // WORLD_HPP
