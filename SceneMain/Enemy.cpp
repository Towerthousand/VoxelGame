#include "Enemy.hpp"
#include "Player.hpp"

Enemy::Enemy(SceneMain* scene, vec3f pos, vec3f scale, Player* targetPlayer) : Entity(scene,pos,scale), targetPlayer(targetPlayer){
}

Enemy::~Enemy() {

}

void Enemy::update(float deltaTime) {
	Entity::update(deltaTime);
	if (norm(targetPlayer->pos-pos) < 50) {
		lookAtPlayer();
	}
	movePos(deltaTime);
}

void Enemy::draw() const {
	Entity::draw();
}

void Enemy::lookAtPlayer() {
	//rotation
	vec3f front = targetPlayer->pos-pos;
	front.y = 0;
	vec3f dummyUp(0,1,0);
	if(norm(cross(dummyUp, front)) != 0) {
		vec3f back = -front;
		normalize(back);
		vec3f right = cross(dummyUp,back);
		normalize(right);
		vec3f up = cross(back,right);
		normalize(up);
		m[0 ] = right.x; m[4 ] = up.x; m[8 ] = back.x; m[12] = 0;
		m[1 ] = right.y; m[5 ] = up.y; m[9 ] = back.y; m[13] = 0;
		m[2 ] = right.z; m[6 ] = up.z; m[10] = back.z; m[14] = 0;
		m[3 ] = 0;       m[7 ] = 0;    m[11] = 0;      m[15] = 1;
	}
}

void Enemy::movePos(float deltaTime) {
	Entity::movePos(deltaTime);
}

