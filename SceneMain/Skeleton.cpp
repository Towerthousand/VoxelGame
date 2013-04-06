#include "Skeleton.hpp"
#include "Player.hpp"
#include "SceneMain.hpp"
#include "Arrow.hpp"

Skeleton::Skeleton(SceneMain* world, vec3f pos, vec3f scale, Player* targetPlayer)
	: Enemy(world, pos, scale, targetPlayer), cooldown(0),
	  shootPosOffset(0,(model.modelHeight*scale.y)/4,0) {
	hitbox.type = Hitbox::BOX;
	hitbox.radius = vec3f(model.modelWidth*scale.x,model.modelHeight*scale.y,model.modelDepth*scale.z)*0.5f;
	acc.y = -40;
}

Skeleton::~Skeleton() {
}

void Skeleton::update(float deltaTime) {
	cooldown -= deltaTime;
	if (norm(targetPlayer->pos-pos) < 20 ) {
		lookAtPlayer();
		if (cooldown <= 0) {
			cooldown = 1;
			Arrow* na = new Arrow(parentScene,(pos+shootPosOffset),vec3f(0.04,0.04,0.04));
			na->vel = targetPlayer->camPos-(pos+shootPosOffset);
			normalize(na->vel);
			na->vel *= 30.0f;
			parentScene->addObject(na);
		}
	}
	movePos(deltaTime);

	vel.x = 0; // Player only accelerates vertically, so speed.x doesn't carry
	vel.y = std::fmax(-70,vel.y);
	vel.z = 0; // Player only accelerates vertically, so speed.z doesn't carry
}

void Skeleton::draw() const {
	model.draw(pos, m,scale);
}

Model Skeleton::model;
