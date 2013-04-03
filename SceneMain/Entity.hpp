#ifndef ENTITY_HPP
#define ENTITY_HPP
#include "GameObject.hpp"

class World;
class Entity : public GameObject { //Stuff that moves (Players, arrows, objects, etc).
								   //Classes that inherit Entity will usally have a hitBox
	public:
		Entity(World &world);
		virtual ~Entity();

		virtual void draw();
		virtual void update(float deltaTime);
		virtual void movePos(float deltaTime);

		vec3f acc; //acceleration
		vec3f vel; //velocity
};

#endif // ENTITY_HPP
