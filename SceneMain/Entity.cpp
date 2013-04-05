#include "Entity.hpp"

Entity::Entity(World& world, vec3f pos) : GameObject(world,pos),
	vel(0,0,0), acc(0,0,0){
}

Entity::~Entity() {
}

void Entity::draw() {
	GameObject::draw();
}

void Entity::update(float deltaTime) {
	GameObject::update(deltaTime);
}

void Entity::movePos(float deltaTime) { //collisons
	vel += acc*deltaTime; //a = const, v = at
	vec3f disp = vel*deltaTime; //deltaX = x0 + vt (intended displacement)

	if (hitbox.collidesWithWorld(vec3f(disp.x,0,0)))
		vel.x = 0;

	if (hitbox.collidesWithWorld(vec3f(0,disp.y,0)))
		vel.y = 0;

	if (hitbox.collidesWithWorld(vec3f(0,0,disp.z)))
		vel.z = 0;

	disp = vel*deltaTime; //corrected displacement
	pos += disp;
}
