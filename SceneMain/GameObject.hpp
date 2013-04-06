#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
#include "tools.hpp"
#include "Hitbox.hpp"

class SceneMain;
class GameObject { //Static objects that have a position and rotation but don't move
	public:
		GameObject(SceneMain* scene, vec3f pos, vec3f scale);
		virtual ~GameObject();

		virtual void update(float deltaTime);
		virtual void draw() const;

		bool isAlive;
		SceneMain* parentScene;
		vec3f pos;
		vec3f scale;
		Hitbox hitbox;
		float m[16];
};

#endif // GAMEOBJECT_HPP
