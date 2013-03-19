#include "Entity.hpp"

Entity::Entity(World& world) : GameObject(world),
	vel(0,0,0), acc(0,0,0){
}

Entity::~Entity() {
}

void Entity::draw() {

}

void Entity::update(float deltaTime) {

}

void Entity::movePos() {

}
