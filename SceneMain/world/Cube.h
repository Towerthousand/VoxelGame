#ifndef CUBE_H
#define CUBE_H

//world voxel
//light = 0 for solid blocks
//MINLIGHT <= light <= MAXLIGHT for non-solid blocks

class Cube {
public:
    unsigned char id;
    unsigned char light;

    inline Cube () : id(0), light(0) {}
    inline Cube (const Cube& b) : id(b.id), light(b.light) {}
    inline Cube (unsigned char id, unsigned char light) : id(id), light(light) {}
    inline Cube (unsigned char id) : id(id), light(0) {}

    inline bool operator==(const Cube& b) const {
        return id == b.id; //comparar la light tambien?
    }
    inline bool operator!=(const Cube& b) const {
        return id != b.id; //comparar la light tambien?
    }
    inline bool operator==(int b) const {
        return id == b;
    }
    inline bool operator!=(int b) const {
        return id != b;
    }
};

#endif // CUBE_H
