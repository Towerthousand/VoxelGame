#ifndef ENTITY_HPP
#define ENTITY_HPP
#include "../GameObject.hpp"

class World;
class Entity : public GameObject { //Stuff that moves (Players, arrows, objects, etc).
								   //Classes that inherit Entity will usally have a hitBox
	public:
		Entity(SceneMain* scene, const vec3f &pos, const vec3f &scale);
		virtual ~Entity();

		virtual void draw() const;
		virtual void update(float deltaTime);
		virtual void updateMatrix();
		virtual void movePos(float deltaTime);

		vec3f vel; //velocity
		vec3f acc; //acceleration
};

#endif // ENTITY_HPP
