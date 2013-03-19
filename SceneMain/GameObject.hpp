#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
#include "tools.hpp"

class World;
class GameObject { //Static objects that have a position and rotation but don't move
	public:
		GameObject(World& world);
		virtual ~GameObject();

		virtual void update();
		virtual void draw();

		vec3f rot;
		vec3f pos;
		World& parentWorld;
};

#endif // GAMEOBJECT_HPP
