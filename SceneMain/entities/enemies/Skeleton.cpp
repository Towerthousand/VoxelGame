#include "Skeleton.hpp"
#include "../items/Arrow.hpp"
#include "../Player.hpp"
#include "../../SceneMain.hpp"
#include "../../../Game.hpp"

Skeleton::Skeleton(SceneMain* world, const vec3f &pos, Player* targetPlayer, const vec3f &scale)
	: Enemy(world, pos, scale, targetPlayer), cooldown(0),
	  shootPosOffset(0,(model.modelHeight*scale.y)/3,0), ambientLight(0) {
	hitbox.type = Hitbox::BOX;
	hitbox.radius = vec3f(model.modelWidth*scale.x,model.modelHeight*scale.y,model.modelDepth*scale.z)*0.5f;
	acc.y = -40;
}

Skeleton::~Skeleton() {
}

void Skeleton::update(float deltaTime) {
	movePos(deltaTime);
	updateMatrix();

	if(parentScene->getWorld()->getCube(pos.x,pos.y,pos.z).ID == 0)
		ambientLight = float(parentScene->getWorld()->getCube(pos.x,pos.y,pos.z).light)/MAXLIGHT;

	cooldown -= deltaTime;
	if (cooldown <= 0) {
		cooldown = 1;
		Arrow* na = new Arrow(parentScene,(pos+shootPosOffset));
		na->vel = targetPlayer->camPos-(pos+shootPosOffset);
		na->vel = glm::normalize(na->vel);
		na->vel *= 70.0f;
		parentScene->addObject(na);
	}

	vel.x = 0; // Mobs only accelerate vertically, so speed.x doesn't carry
	vel.y = std::fmax(-70,vel.y);
	vel.z = 0; // Mobs only accelerate vertically, so speed.z doesn't carry
}

void Skeleton::draw() const{
	mat4f poppedMat = parentScene->getState().model;
	parentScene->getState().model = modelMatrix;
	parentScene->getState().updateShaderUniforms(parentScene->getShader("MODEL"));
	parentScene->getShader("MODEL").sendUniform1f("ambientLight",ambientLight);
	model.draw();
	parentScene->getState().model = poppedMat;
}

void Skeleton::updateMatrix() {
	modelMatrix = glm::translate(mat4f(1.0),vec3f(pos.x,pos.y,pos.z));
	lookAtPlayer(); //rotates matrix so that it looks at the player in x and z coords
	vec3f radius = vec3f(model.modelWidth*scale.x,
						 model.modelHeight*scale.y,
						 model.modelDepth*scale.z)*0.5f;
	modelMatrix = glm::translate(modelMatrix,vec3f(-radius.x,-radius.y,-radius.z)); //translate to center, after rotation
	modelMatrix = glm::scale(modelMatrix,vec3f(scale.x,scale.y,scale.z));
}

Model Skeleton::model;
