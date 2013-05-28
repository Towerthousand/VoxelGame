#include "Octree.h"
#include <iostream>
#include "tools.hpp"

using namespace std;

Cube Octree::get(int x, int y, int z, int size) const
{
    if(isFilled()) return b;

    int halfsize = size-1;
    int xx = (x >> halfsize) & 1;
    int yy = (y >> halfsize) & 1;
    int zz = (z >> halfsize) & 1;
    int ind = xx | (yy<<1) | (zz<<2);

    return children[ind]->get(x, y, z, size-1);
}

void Octree::set(int x, int y, int z, int size, Cube b2)
{
    if(isFilled() && b == b2)
        return;

    if(size == 0)
    {
        if(b != b2)
        {
            b = b2;
            dirty = true;
        }
        return;
    }

    //Subdividir if needed.
    if(isFilled())
        for(int i = 0; i < 8; i++)
            children[i] = new Octree(b);

    //Setear el hijo
    int halfsize = size-1;
    int xx = (x >> halfsize) & 1;
    int yy = (y >> halfsize) & 1;
    int zz = (z >> halfsize) & 1;
    int ind = xx | (yy<<1) | (zz<<2);

    children[ind]->set(x, y, z, size-1, b2);
    dirty |= children[ind]->dirty;

    //Juntar if needed.
    bool same = children[0]->isFilled();
    for(int i = 1; i < 8 && same; i++)
        if(!children[i]->isFilled() || children[i]->b != children[0]->b)
            same = false;

    if(same)
    {
        b = children[0]->b;
        for(int i = 0; i < 8; i++)
        {
            delete children[i];
            children[i] = NULL;
        }
    }
}


void Octree::draw(int x, int y, int z, int size) const
{
    if(chunk != NULL)
        chunk->draw();

    if(isFilled())
        return;

    int inc = 1 << (size-1);
    for(int i = 0; i < 8; i++)
        children[i]->draw(x+inc*((i >> 0)&1), y+inc*((i >> 1)&1), z+inc*((i >> 2)&1), size-1);
}

void Octree::update(int x, int y, int z, int size, SceneMain* scene)
{
    if(!dirty)
        return;

    dirty = false;
    if(size == CHUNKSIZE_POW2)
    {
        if(chunk == NULL)
            chunk = new Chunk(x, y, z, size, this, scene);
        else
            chunk->redraw();
    }

    if(isFilled())
        return;

    int inc = 1 << (size-1);
    for(int i = 0; i < 8; i++)
        children[i]->update(x+inc*((i >> 0)&1), y+inc*((i >> 1)&1), z+inc*((i >> 2)&1), size-1, scene);
}
