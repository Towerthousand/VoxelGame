#include "Entity.hpp"

Entity::Entity(SceneMain* scene, vec3f pos, vec3f scale) : GameObject(scene,pos,scale),
	vel(0,0,0), acc(0,0,0){
}

Entity::~Entity() {
}

void Entity::draw() const {
	GameObject::draw();
}

void Entity::update(float deltaTime) {
	GameObject::update(deltaTime);
}

void Entity::movePos(float deltaTime) { //collisons
	vel += acc*deltaTime; //a = const, v = at
	vec3f disp = vel*deltaTime; //deltaX = x0 + vt (intended displacement)
	if(hitbox.collidesWithWorld(vec3f(disp.x,0,0))) {
		float min = 0;
		float max = 1;
		while(max-min > 0.001)
		{
			float m = (max+min)/2;
			if(hitbox.collidesWithWorld(vec3f(disp.x*m,0,0)))
				max = m;
			else
				min = m;
		}
		vel.x = 0;
		disp.x *= min;
	}

	if(hitbox.collidesWithWorld(vec3f(0,disp.y,0))) {
		float min = 0;
		float max = 1;
		while(max-min > 0.001)
		{
			float m = (max+min)/2;
			if(hitbox.collidesWithWorld(vec3f(0,disp.y*m,0)))
				max = m;
			else
				min = m;
		}
		vel.y = 0;
		disp.y *= min;
	}

	if(hitbox.collidesWithWorld(vec3f(0,0,disp.z))) {
		float min = 0;
		float max = 1;
		while(max-min > 0.001)
		{
			float m = (max+min)/2;
			if(hitbox.collidesWithWorld(vec3f(0,0,disp.z*m)))
				max = m;
			else
				min = m;
		}
		vel.z = 0;
		disp.z *= min;
	}

	pos += disp;
}
