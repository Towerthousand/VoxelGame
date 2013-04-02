#include "Entity.hpp"

Entity::Entity(World& world) : GameObject(world),
	vel(0,0,0), acc(0,0,0), hitbox(vec3f(0,0,0), vec3f(0,0,0)){
}

Entity::~Entity() {
}

void Entity::draw() {

}

void Entity::update(float deltaTime) {

}

void Entity::movePos(float deltaTime) { //collisons

	vec3f disp = vel*deltaTime; //deltax = vt bitch

	if (!hitbox.collidesWithWorld(parentWorld,vec3f(disp.x,0,0)))
		pos.x += disp.x;
	if (!hitbox.collidesWithWorld(parentWorld,vec3f(0,disp.y,0)))
		pos.y += disp.y;
	if (!hitbox.collidesWithWorld(parentWorld,vec3f(0,0,disp.z)))
		pos.z += disp.z;

	vel.y = std::fmin(10,vel.y);
}
