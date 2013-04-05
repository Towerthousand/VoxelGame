#include "GameObject.hpp"
#include "World.hpp"

GameObject::GameObject(World& world, vec3f pos) : parentWorld(world),
	pos(pos), scale(0,0,0), hitbox(*this, Hitbox::POINT, vec3f(0,0,0)) {
}

GameObject::~GameObject() {
}

void GameObject::update(float deltaTime) {
}

void GameObject::draw() {
}
