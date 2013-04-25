#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
#include "tools.hpp"
#include "Hitbox.hpp"

class SceneMain;
class GameObject { //Static objects that have a position and rotation but don't move
	public:
		GameObject(SceneMain* scene, const vec3f &pos, const vec3f &scale);
		virtual ~GameObject();

		virtual void update(float deltaTime);
		virtual void updateMatrix();
		virtual void draw() const;

		bool isAlive;
		vec3f pos;
		vec3f scale;
		Hitbox hitbox;
	protected:
		SceneMain* parentScene;
		mat4f modelMatrix;
		friend class Hitbox;
};

#endif // GAMEOBJECT_HPP
