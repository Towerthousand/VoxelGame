#include "Enemy.hpp"
#include "../Player.hpp"

Enemy::Enemy(SceneMain* scene, const vec3f &pos, const vec3f &scale, Player* targetPlayer) : Entity(scene,pos,scale), targetPlayer(targetPlayer){
}

Enemy::~Enemy() {

}

void Enemy::update(float deltaTime) {
	Entity::update(deltaTime);
	if (glm::length(targetPlayer->pos-pos) < 50) {
		lookAtPlayer();
	}
	movePos(deltaTime);
}

void Enemy::draw() const {
	Entity::draw();
}

void Enemy::movePos(float deltaTime) {
	Entity::movePos(deltaTime);
}

void Enemy::updateMatrix() {
	Entity::updateMatrix();
}

void Enemy::lookAtPlayer() { //rotates current model matrix to look at player
	//rotation
	vec3f front = targetPlayer->pos-pos;
	front.y = 0;
	vec3f dummyUp(0,1,0);
	if(glm::length(glm::cross(dummyUp,front)) != 0) {
		vec3f back = -front;
		back = glm::normalize(back);
		vec3f right = glm::cross(dummyUp,back);
		right = glm::normalize(right);
		vec3f up = glm::cross(back,right);
		up = glm::normalize(up);
		modelMatrix *= mat4f(right.x, right.y, right.z, 0,
							 up.x   , up.y   , up.z   , 0,
							 back.x , back.y , back.z , 0,
							 0      , 0      , 0      , 1);
	}
}
