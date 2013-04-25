#include "Arrow.hpp"
#include "../Player.hpp"
#include "../../SceneMain.hpp"
#include "../../../Game.hpp"

Arrow::Arrow(SceneMain* scene, const vec3f &pos, const vec3f &scale) :
	Entity(scene,pos,scale) {
	this->acc = vec3f(0,GRAVITY,0);
	this->pos = pos;
	this->scale = scale;
	this->hitbox.type = Hitbox::POINT;
}

Arrow::~Arrow() {
}

void Arrow::update(float deltaTime) {
	movePos(deltaTime);
}
void Arrow::updateMatrix() {
	vec3f offset(-modelMatrix(0,2)*0.6,-modelMatrix(1,2)*0.6,-modelMatrix(2,2)*0.6);//offset determines arrow's notch position
	if(!hitbox.collidesWithWorld(offset)) {
		modelMatrix = mat4f::fromIdentity();
		modelMatrix.translate(pos.x,pos.y,pos.z);
		//rotation
		vec3f dummyUp(0,1,0);
		if((dummyUp^vel).module() != 0) {
			vec3f back = -vel;
			back.normalize();
			vec3f dummyUp(0,1,0);
			vec3f right = dummyUp^back;
			right.normalize();
			vec3f up = back^right;
			up.normalize();
			modelMatrix *= mat4f(right.x, up.x, back.x, 0,
								 right.y, up.y, back.y, 0,
								 right.z, up.z, back.z, 0,
								 0      , 0   , 0     , 1);
		}
		else { //no x or z velocity, fall straight down
			modelMatrix *= mat4f(1, 0, 0, 0,
								 0, 1, 0, 0,
								 0, 0, 1, 0,
								 0, 0, 0, 1);
		}
		vec3f radius = vec3f(model.modelWidth*scale.x,
							 model.modelHeight*scale.y,
							 model.modelDepth*scale.z)*0.5f;
		modelMatrix.translate(-radius.x,-radius.y,-radius.z); //translate to center, after rotation
		modelMatrix.scale(scale.x,scale.y,scale.z);
	}
}

void Arrow::draw() const {
	mat4f poppedMat = parentScene->getState().model;
	parentScene->getState().model = modelMatrix;
	parentScene->getState().updateShaderUniforms(parentScene->getShader("MODEL"));
	parentScene->getShader("MODEL").sendUniform1f("shadow",1-float(parentScene->getWorld().getCube(pos.x,pos.y,pos.z).light)/MAXLIGHT);
	model.draw();
	parentScene->getState().model = poppedMat;
}

void Arrow::movePos(float deltaTime) {
	vec3f offset(-modelMatrix(0,2)*0.6,-modelMatrix(1,2)*0.6,-modelMatrix(2,2)*0.6);//offset determines arrow's notch position
	if(!hitbox.collidesWithWorld(offset)) {
		//Position
		vel += acc*deltaTime; //a = const, v = at
		vec3f disp = vel*deltaTime;
		pos += disp; //before checking for collisions, so that if it collides it gets stuck.
		if (hitbox.collidesWithWorld(disp+offset)) {
			vel.x = 0;
			vel.y = 0;
			vel.z = 0;
			acc.y = 0;
			pos += disp; //get a bit more stuck
		}
		else
			acc.y = -40;
		vel.y = std::fmax(-70,vel.y);
	}
}

Model Arrow::model = Model();
