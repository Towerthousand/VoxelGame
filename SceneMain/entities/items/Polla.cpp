#include "Polla.hpp"
#include "../Player.hpp"
#include "../../SceneMain.hpp"
#include "../../../Game.hpp"

Polla::Polla(SceneMain* scene, const vec3f &pos, Player *player, const vec3f &scale) :
	Entity(scene,pos,scale), followedPlayer(player), ambientLight(0) {
	this->hitbox.type = Hitbox::POINT;
}

Polla::~Polla() {
}

void Polla::update(float deltaTime) {
	movePos(deltaTime);
	updateMatrix();
	if(parentScene->getWorld().getCube(pos.x,pos.y,pos.z).ID == 0)
		ambientLight = float(parentScene->getWorld().getCube(pos.x,pos.y,pos.z).light)/MAXLIGHT;
}

void Polla::draw() const{
	mat4f poppedMat = parentScene->getState().model;
	parentScene->getState().model = modelMatrix;
	parentScene->getState().updateShaderUniforms(parentScene->getShader("MODEL"));
	parentScene->getShader("MODEL").sendUniform1f("ambientLight",ambientLight);
	model.draw();
	parentScene->getState().model = poppedMat;
}

void Polla::updateMatrix() {
	if(!hitbox.collidesWithWorld()) {
		modelMatrix = mat4f::fromIdentity();
		modelMatrix.translate(pos.x,pos.y,pos.z);
		//rotation
		vec3f dummyUp(0,1,0);
		if((dummyUp^vel).module() != 0) {
			vec3f back = -vel;
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
		vec3f radius = vec3f(model.modelWidth*scale.x,
							 model.modelHeight*scale.y,
							 model.modelDepth*scale.z)*0.5f;
		modelMatrix.translate(-radius.x,-radius.y,-radius.z); //translate to center, after rotation
		modelMatrix.scale(scale.x,scale.y,scale.z);
	}
}

void Polla::movePos(float deltaTime) {
	vel += vec3f((followedPlayer->pos.x-pos.x+0.6),
				 (followedPlayer->pos.y-pos.y+0.6),
				 (followedPlayer->pos.z-pos.z+0.6)) * deltaTime;
	vec3f disp = vel*deltaTime; //deltaX = x0 + vt (intended displacement)
	if (hitbox.collidesWithWorld(vec3f(disp.x,0,0)) ||
		hitbox.collidesWithWorld(vec3f(0,disp.y,0)) ||
		hitbox.collidesWithWorld(vec3f(0,0,disp.z))) {
		vel.x = 0;
		vel.y = 0;
		vel.z = 0;
	}
	disp = vel*deltaTime; //corrected displacement
	disp.y += std::sin(GLOBALCLOCK.getElapsedTime().asSeconds()*10)/50;
	pos += disp;
}

Model Polla::model = Model();
