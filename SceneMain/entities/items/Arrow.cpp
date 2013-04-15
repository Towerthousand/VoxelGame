#include "Arrow.hpp"
#include "../Player.hpp"

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

void Arrow::draw() const {
	model.draw(pos-hitbox.radius,m,scale);
}

void Arrow::movePos(float deltaTime) {
	vec3f offset(-m[8]*0.6,-m[9]*0.6,-m[10]*0.6);//offset determines arrow's not position
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
			m[0 ] = right.x; m[4 ] = up.x; m[8 ] = back.x; m[12] = 0;
			m[1 ] = right.y; m[5 ] = up.y; m[9 ] = back.y; m[13] = 0;
			m[2 ] = right.z; m[6 ] = up.z; m[10] = back.z; m[14] = 0;
			m[3 ] = 0;       m[7 ] = 0;    m[11] = 0;      m[15] = 1;
		}
		else if(!hitbox.collidesWithWorld(offset)){ //no x or z velocity, fall straight down
			m[0 ] = 1; m[4 ] = 0; m[8 ] = 0; m[12] = 0;
			m[1 ] = 0; m[5 ] = 0; m[9 ] = 1; m[13] = 0;
			m[2 ] = 0; m[6 ] =-1; m[10] = 0; m[14] = 0;
			m[3 ] = 0; m[7 ] = 0; m[11] = 0; m[15] = 1;
		}
	}
}

Model Arrow::model = Model();
