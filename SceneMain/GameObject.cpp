#include "GameObject.hpp"
#include "SceneMain.hpp"

GameObject::GameObject(SceneMain* scene, const vec3f &pos, const vec3f &scale) :
	isAlive(true), pos(pos), scale(scale),
	hitbox(this, Hitbox::POINT, vec3f(0,0,0)),
	parentScene(scene),
	modelMatrix(mat4f::fromIdentity()) {
}

GameObject::~GameObject() {
}

void GameObject::update(float deltaTime) {
}

void GameObject::updateMatrix() {
}

void GameObject::draw() const {
}
