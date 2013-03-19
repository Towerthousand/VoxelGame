#include "GameObject.hpp"
#include "World.hpp"

GameObject::GameObject(World& world) : parentWorld(world),
	pos(0,0,0), rot(0,0,0) {
}

GameObject::~GameObject() {
}

void GameObject::update() {

}

void GameObject::draw() {

}
