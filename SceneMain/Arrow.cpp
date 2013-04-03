#include "Arrow.hpp"

Arrow::Arrow(World &world, vec3f pos, vec3f rot, vec3f scale) :
	Entity(world) , model("resources/arrow.vox") {
	this->acc = vec3f(0,-40,0);
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;
	hitbox = Hitbox(pos,vec3f((12*this->scale.x)/2,
							  (3*this->scale.y)/2,
							  (3*this->scale.z)/2));
}

Arrow::~Arrow() {
}

void Arrow::update(float deltaTime) {
	movePos(deltaTime);
}

void Arrow::draw() {
	model.draw(pos-hitbox.radius,rot,scale);
}

void Arrow::movePos(float deltaTime) { //collisons
	vel += acc*deltaTime; //a = const, v = at
	vec3f disp = vel*deltaTime; //deltaX = x0 + vt (intended displacement)

	if (hitbox.collidesWithWorld(parentWorld,vec3f(disp.x,0,0)) ||
		hitbox.collidesWithWorld(parentWorld,vec3f(0,disp.y,0)) ||
		hitbox.collidesWithWorld(parentWorld,vec3f(0,0,disp.z))) {
		vel.x = 0;
		vel.y = 0;
		vel.z = 0;
	}

	disp = vel*deltaTime; //corrected displacement
	pos += disp;

	hitbox.pos = pos; //hitbox must always be at player's pos
}
//Model Arrow::model = Model("resources/arrow.vox");
