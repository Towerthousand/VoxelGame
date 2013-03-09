#include "Camera.hpp"
#include "World.hpp"

//
// This implementation of "camera" has no z rotation (fuck quaternions)
// and is thougt of as a no-fly, fps kind of view. Tweak it!
//

Camera::Camera(World &world) :
	pos(sf::Vector3f(0,75,0)),
	rot(sf::Vector3f(0,0,0)),
	parentWorld(world) {
}

Camera::~Camera() {
}

void Camera::movePos(const sf::Vector3f &disp) {
	if(parentWorld.getCubeAbs(floor(pos.x + disp.x),floor(pos.y + PLAYER_HEIGHT),floor(pos.z)).ID == 0)
		pos.x += disp.x;
	if(parentWorld.getCubeAbs(floor(pos.x),floor(pos.y + disp.y + PLAYER_HEIGHT),floor(pos.z)).ID == 0)
		pos.y += disp.y;
	if(parentWorld.getCubeAbs(floor(pos.x),floor(pos.y + PLAYER_HEIGHT),floor(pos.z + disp.z)).ID == 0)
		pos.z += disp.z;
}

void Camera::rotateX(float deg) {
	rot.x += deg;

	//Euler fix
	if (rot.x > 89.9)
		rot.x = 89.9;
	else if (rot.x < -89.9)
		rot.x = -89.9;
}

void Camera::rotateY(float deg) {
	rot.y += deg;

	//prevent overflow of float rot.y
	if (rot.y > 360)
		rot.y = rot.y - 360;
	else if (rot.y < -360)
		rot.y = rot.y + 360;
}
