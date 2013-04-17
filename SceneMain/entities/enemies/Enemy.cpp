#include "Enemy.hpp"
#include "../Player.hpp"

Enemy::Enemy(SceneMain* scene, const vec3f &pos, const vec3f &scale, Player* targetPlayer) : Entity(scene,pos,scale), targetPlayer(targetPlayer){
}

Enemy::~Enemy() {

}

void Enemy::update(float deltaTime) {
	Entity::update(deltaTime);
	if ((targetPlayer->pos-pos).module() < 50) {
		lookAtPlayer();
	}
	movePos(deltaTime);
}


void Enemy::updateMatrix() {
	Entity::updateMatrix();
}

void Enemy::draw() const {
	Entity::draw();
}

void Enemy::lookAtPlayer() { //rotates current model matrix to look at player
	//rotation
	vec3f front = targetPlayer->pos-pos;
	front.y = 0;
	vec3f dummyUp(0,1,0);
	if((dummyUp^front).module() != 0) {
		vec3f back = -front;
		back.normalize();
		vec3f right = dummyUp^back;
		right.normalize();
		vec3f up = back^right;
		up.normalize();
		modelMatrix *= mat4f(right.x, up.x, back.x, 0,
							 right.y, up.y, back.y, 0,
							 right.z, up.z, back.z, 0,
							 0      , 0   , 0     , 1);
	}
}

void Enemy::movePos(float deltaTime) {
	Entity::movePos(deltaTime);
}

