#include "Octree.h"
#include <iostream>
#include "tools.hpp"
#include "../SceneMain.hpp"
#include "../../RenderState.hpp"
using namespace std;


const int vertexPoints[8][3] = {
    {0,0,1},
    {1,0,1},
    {1,1,1},
    {0,1,1},
    {0,0,0},
    {1,0,0},
    {1,1,0},
    {0,1,0}
};

const int indexes[24] = {
    0,1,
    1,2,
    2,3,
    3,0,
    4,5,
    5,6,
    6,7,
    7,4,
    0,4,
    1,5,
    2,6,
    3,7,
};


void drawWireCube(int x, int y, int z, int size, SceneMain* parentScene)  {
    mat4f poppedMat = parentScene->getState().model;
    parentScene->getState().model =
            glm::translate(parentScene->getState().model,
                           vec3f(x+0.2, y+0.2, z+0.2));
    int s = 1<<size;
    parentScene->getState().model =
            glm::scale(parentScene->getState().model,vec3f(s-0.4, s-0.4, s-0.4));
    parentScene->getState().updateShaderUniforms(parentScene->getShader("MODEL"));
    parentScene->getShader("MODEL").use();
    glLineWidth(3.5);
    glEnableClientState(GL_VERTEX_ARRAY);
    size -= 4;
    glColor4f((size>>2)&1,(size>>1)&1,(size>>0)&1,1.0);
//    glColor4f(1.0, 0.0, 0.0, 0.5);
    glVertexPointer(3, GL_INT, 0, &vertexPoints[0]);
    glDrawElements(GL_LINES,24,GL_UNSIGNED_INT,&indexes[0]);
    glDisableClientState(GL_VERTEX_ARRAY);
    glColor4f(1.0,1.0,1.0,1.0);
    parentScene->getState().model = poppedMat;
}





//=================================


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
            children[i] = nullptr;
        }
    }
}


void Octree::draw(int x, int y, int z, int size) const
{
    if(chunk != nullptr)
    {
//        drawWireCube(x, y, z, size, chunk->parentScene);
        chunk->draw();
    }

    if(isFilled())
        return;

    int halfsize = 1 << (size-1);

    if(size > CHUNKSIZE_POW2)
        for(int i = 0; i < 8; i++)
            children[i]->draw(x+halfsize*((i >> 0)&1), y+halfsize*((i >> 1)&1), z+halfsize*((i >> 2)&1), size-1);
}

void calcdist(long long halfsize, long long x, long long& neard, long long& fard)
{
    if(abs(x) < halfsize)
        neard = 0;
    else
        neard = min(abs(x-halfsize), abs(x+halfsize));

    fard = max(abs(x-halfsize), abs(x+halfsize));
}

int calcprecision(long long dist)
{
    if(dist == 0) return 0;
    if(dist < 0)
        cout<<"Wtf: calcprecision(dist<0)\n";

    int prec = 64-__builtin_clzl(dist);
    return max(0, prec/2 - 6);
}

long long combineDist(long long x, long long y, long long z)
{
    long long maxor = 1<<30;
    if(x > maxor || y > maxor || z > maxor)
        return 1LL<<60; //inf

    return x*x+y*y+z*z;
    /*
    long long d1 = x+y+z;
    long long d2 = max(max(x, y), z);
    return (d1+d2)/2;*/
}

void Octree::update(int x, int y, int z, int size, SceneMain* scene, int px, int py, int pz)
{

    if(isFilled())
        return;

    long long halfsize = 1 << (size-1);

    //TODO: Make absolutely sure that there are no overflows.

    long long xnear, ynear, znear;
    long long xfar, yfar, zfar;

    calcdist(halfsize, x+halfsize-px, xnear, xfar);
    calcdist(halfsize, y+halfsize-py, ynear, yfar);
    calcdist(halfsize, z+halfsize-pz, znear, zfar);

    long long dnear = combineDist(xnear, ynear, znear);
    long long dfar = combineDist(xfar, yfar, zfar);

    int pnear = calcprecision(dnear);
    int pfar = calcprecision(dfar);

    //If this happens, something's very wrong.

//    std::cout<<"update "<<x<<" "<<y<<" "<<z<<" "<<size<<": "<<dnear<<" "<<dfar<<": "<<pnear<<" "<<pfar<<"\n";

//    if(pnear > pfar)
//        std::cout<<"Wtf: pnear > pfar\n";
//    if(dnear > dfar)
//        std::cout<<"Wtf: dnear > dfar\n";

    //dirty = false;

    if(abs(pnear-pfar) <= 1 || size <= pnear+CHUNKSIZE_POW2)
    {
        pnear = min(pnear, size-CHUNKSIZE_POW2);
        renderToPrecision(x, y, z, size, pnear, scene);
    }
    else
    {
        if(chunk != NULL)
        {
            delete chunk;
            chunk = NULL;
        }

        for(int i = 0; i < 8; i++)
            children[i]->update(x+halfsize*((i >> 0)&1), y+halfsize*((i >> 1)&1), z+halfsize*((i >> 2)&1), size-1, scene, px, py, pz);
    }
}

void Octree::renderToPrecision(int x, int y, int z, int size, int precision, SceneMain* scene)
{
//    std::cout<<"RenderToPrecision "<<x<<" "<<y<<" "<<z<<" "<<size<<" "<<precision<<"\n";

    if(size == CHUNKSIZE_POW2+precision)
    {
        if(chunk == NULL)
            chunk = new Chunk(x, y, z, size, this, scene);
        else if(dirty)
            chunk->redraw();
    }
    else
    {
        if(chunk != NULL)
        {
            delete chunk;
            chunk = NULL;
        }
    }

    dirty = false;

    //dirty = false;
    if(isFilled()) return;

    if(size > CHUNKSIZE_POW2)
    {
        int halfsize = 1 << (size-1);

        for(int i = 0; i < 8; i++)
            children[i]->renderToPrecision(x+halfsize*((i >> 0)&1), y+halfsize*((i >> 1)&1), z+halfsize*((i >> 2)&1), size-1, precision, scene);
    }
}


