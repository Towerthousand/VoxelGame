#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
#include "tools.hpp"
#include "Hitbox.hpp"

class World;
class GameObject { //Static objects that have a position and rotation but don't move
	public:
		GameObject(World& world, vec3f pos);
		virtual ~GameObject();

		virtual void update(float deltaTime);
		virtual void draw();

		World& parentWorld;
		vec3f pos;
		vec3f scale;
		Hitbox hitbox;
		float m[16];
};

#endif // GAMEOBJECT_HPP
