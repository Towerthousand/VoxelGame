#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
#include "tools.hpp"
#include "Hitbox.hpp"

class World;
class GameObject { //Static objects that have a position and rotation but don't move
	public:
		GameObject(World& world);
		virtual ~GameObject();

		virtual void update(float deltaTime);
		virtual void draw();

		World& parentWorld;
		vec3f rot;
		vec3f pos;
		vec3f scale;
		Hitbox hitbox;
};

#endif // GAMEOBJECT_HPP
