#ifndef ENTITY_H
#define ENTITY_H
#include "Camera.hpp"

class World;
class Entity : public Camera {
	public:
		Entity(World &world);
		~Entity();

		virtual void draw() = 0;
		virtual void update(float deltaTime) = 0;
};

#endif // ENTITY_H
