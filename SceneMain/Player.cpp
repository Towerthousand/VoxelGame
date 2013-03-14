
#include "Player.hpp"
#include "World.hpp"

Player::Player(World &world) : Camera(world) {
	vel = sf::Vector3f(0, 0, 0);
	selectedID = 1;
}

Player::~Player() {
}

void Player::update(float deltaTime) {
	vel.y -= 15.0f*deltaTime;
    movePos(vel);
    //calculate frustrum
}

void Player::draw() const {
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glColor3f(1,1,1);
	glVertex3f(-0.0002,      0,-0.01);
	glVertex3f( 0.0002,      0,-0.01);
	glVertex3f(      0,-0.0002,-0.01);
	glVertex3f(      0, 0.0002,-0.01);
	glEnd();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}
