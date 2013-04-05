#include "Skeleton.hpp"

Skeleton::Skeleton(World &world, vec3f pos, Player &targetPlayer) : Enemy(world, pos,targetPlayer) {
	hitbox.type = Hitbox::BOX;
	hitbox.radius = vec3f(0.2,0.9,0.2);
}

Skeleton::~Skeleton() {
}

void Skeleton::update(float deltaTime) {
	Enemy::update(deltaTime);
	if (norm(targetPlayer.pos-pos) < 50) {
	}
	vel.x = 0; // Player only accelerates vertically, so speed.x doesn't carry
	vel.y = std::fmax(-70,vel.y);
	vel.z = 0; // Player only accelerates vertically, so speed.z doesn't carry
}

void Skeleton::draw() {

}
