#include "GameObject.hpp"
#include "World.hpp"

GameObject::GameObject(World& world) : parentWorld(world),
	pos(0,0,0), rot(0,0,0), scale(0,0,0), hitbox(*this, vec3f(0,0,0)) {
}

GameObject::~GameObject() {
}

void GameObject::update(float deltaTime) {
}

void GameObject::draw() {
}
