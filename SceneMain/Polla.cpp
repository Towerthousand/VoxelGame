#include "Polla.hpp"
#include "Player.hpp"

Polla::Polla(SceneMain* scene, vec3f pos, vec3f scale, Player *player) :
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
	model.draw(pos-hitbox.radius,m,scale);
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
	if(norm(cross(dummyUp, vel)) != 0) {
		vec3f back = -vel;
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

Model Polla::model = Model();
