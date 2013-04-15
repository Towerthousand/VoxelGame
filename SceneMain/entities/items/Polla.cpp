#include "Polla.hpp"
#include "../Player.hpp"

Polla::Polla(SceneMain* scene, const vec3f &pos, Player *player, const vec3f &scale) :
	Entity(scene,pos,scale), followedPlayer(player) {
	this->hitbox.type = Hitbox::POINT;
}

Polla::~Polla() {
}

void Polla::update(float deltaTime) {
	vel += vec3f((followedPlayer->pos.x-pos.x+0.6)/100,(followedPlayer->pos.y-pos.y+0.6)/100,(followedPlayer->pos.z-pos.z+0.6)/100);
	movePos(deltaTime);
}

void Polla::draw() const{
	vec3f newScale = scale;
	newScale.z *= std::abs(std::sin(GLOBALCLOCK.getElapsedTime().asSeconds()*5));
	model.draw(pos-hitbox.radius,modelMatrix,newScale);
}

void Polla::movePos(float deltaTime) {

	//Position
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

	//rotation
	vec3f dummyUp(0,1,0);
	if((dummyUp^vel).module() != 0) {
		vec3f back = -vel;
		back.normalize();
		vec3f right = dummyUp^back;
		right.normalize();
		vec3f up = back^right;
		up.normalize();
		modelMatrix = mat4f(right.x, up.x, back.x, 0,
							right.y, up.y, back.y, 0,
							right.z, up.z, back.z, 0,
							0      , 0   , 0     , 1);
	}
}

Model Polla::model = Model();
