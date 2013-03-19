#ifndef ENTITY_HPP
#define ENTITY_HPP
#include "GameObject.hpp"

class World;
class Entity : public GameObject {
	public:
		Entity(World &world);
		virtual ~Entity();

		virtual void draw();
		virtual void update(float deltaTime);

		virtual void movePos();

		std::vector<vec3f> hitBox;
};

#endif // ENTITY_HPP
