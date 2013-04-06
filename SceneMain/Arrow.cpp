#include "Arrow.hpp"
#include "Player.hpp"

Arrow::Arrow(SceneMain* scene, vec3f pos, vec3f scale) :
	Entity(scene,pos,scale) {
	this->acc = vec3f(0,-10,0);
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
	//Position
	vel += acc*deltaTime; //a = const, v = at
	vec3f disp = vel*deltaTime; //deltaX = x0 + vt (intended displacement)
	acc.y = -10;
	if (hitbox.collidesWithWorld(vec3f(disp.x,0,0)) ||
		hitbox.collidesWithWorld(vec3f(0,disp.y,0)) ||
		hitbox.collidesWithWorld(vec3f(0,0,disp.z))) {
		vel.x = 0;
		vel.y = 0;
		vel.z = 0;
		acc.y = 0;
	}
	disp = vel*deltaTime; //corrected displacement
	pos += disp;
	vel.y = std::fmax(-70,vel.y);

	//rotation
	vec3f dummyUp(0,1,0);
	if(norm(cross(dummyUp, vel)) != 0) {
		vec3f back = -vel;
		normalize(back);
		vec3f dummyUp(0,1,0);
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

Model Arrow::model = Model();
