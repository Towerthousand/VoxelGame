#ifndef ENTITY_H
#define ENTITY_H
#include "Camera.hpp"

class World;
class Entity : public Camera {
    public:
        Entity();
        ~Entity();

        virtual draw() = 0;
        virtual update() = 0;
};

#endif // ENTITY_H
