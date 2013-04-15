#include "GameObject.hpp"
#include "SceneMain.hpp"

GameObject::GameObject(SceneMain* scene, const vec3f &pos, const vec3f &scale) :
	isAlive(true), parentScene(scene),
	pos(pos), scale(scale), hitbox(this, Hitbox::POINT, vec3f(0,0,0)) {
	modelMatrix = mat4f(1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1);
}

GameObject::~GameObject() {
}

void GameObject::update(float deltaTime) {
}

void GameObject::draw() const {
}
