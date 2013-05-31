#ifndef OCTREE_H
#define OCTREE_H

#include<cstdlib>
#include "Cube.h"
#include "Chunk.hpp"

class Chunk;
class SceneMain;

class Octree
{
public:
    Octree() : b(0), children{NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}, chunk(NULL), dirty(false) {}
    Octree(Cube b) : b(b), children{NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}, chunk(NULL), dirty(false) {}

    ~Octree()
    {
        if(!isFilled())
            for(int i = 0; i < 8; i++)
                delete children[i];
        if(chunk != NULL)
            delete chunk;
    }

    Cube b;
    Octree* children[8];
    Chunk* chunk;
    bool dirty;

    Cube get(int x, int y, int z, int size) const;
    void set(int x, int y, int z, int size, Cube b2);
    void draw(int x, int y, int z, int size) const;
    void update(int x, int y, int z, int size, SceneMain* scene, int px, int py, int pz);
    void renderToPrecision(int x, int y, int z, int size, int precision, SceneMain* scene);

    inline bool isFilled() const
    {
        return children[0] == NULL;
    }
};

#endif // OCTREE_H
