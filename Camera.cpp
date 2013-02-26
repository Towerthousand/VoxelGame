#include "Camera.hpp"
#include "World.hpp"

//
// This implementation of "camera" has no z rotation (fuck quaternions)
// and is thougt of as a no-fly, fps kind of view. Tweak it!
//

Camera::Camera(const World &world) :
  pos(sf::Vector3f(0,75,0)),
  rot(sf::Vector3f(0,0,0)),
  parentWorld(world) {
}

Camera::~Camera() {
}

void Camera::moveX(const float& distance) {
	if(parentWorld.getCubeAbs(floor(pos.x + cos(rot.y*DEG_TO_RAD)*distance),floor(pos.y + PLAYER_HEIGHT),floor(pos.z)) == 0)
		pos.x += cos(rot.y*DEG_TO_RAD) * distance;
	if(parentWorld.getCubeAbs(floor(pos.x),floor(pos.y + PLAYER_HEIGHT),floor(pos.z + sin(rot.y*DEG_TO_RAD)*distance)) == 0)
		pos.z += sin(rot.y*DEG_TO_RAD) * distance;
}

void Camera::moveY(const float& distance) {
	if(parentWorld.getCubeAbs(floor(pos.x),floor(pos.y + distance + PLAYER_HEIGHT),floor(pos.z)) == 0)
		pos.y += distance;
}

void Camera::moveZ(const float& distance) {
	if(parentWorld.getCubeAbs(floor(pos.x + sin(rot.y*DEG_TO_RAD)*distance),floor(pos.y + PLAYER_HEIGHT),floor(pos.z)) == 0)
		pos.x += sin(rot.y*DEG_TO_RAD) * distance;
	if(parentWorld.getCubeAbs(floor(pos.x),floor(pos.y + PLAYER_HEIGHT),floor(pos.z - cos(rot.y*DEG_TO_RAD)*distance)) == 0)
		pos.z -= cos(rot.y*DEG_TO_RAD) * distance;
}
void Camera::rotateX(const float& deg) {
	rot.x += deg;

	//Euler fix
	if (rot.x > 89.9)
		rot.x = 89.9;
	else if (rot.x < -89.9)
		rot.x = -89.9;
}

void Camera::rotateY(const float &deg) {
	rot.y += deg;

	//prevent overflow of float rot.y
	if (rot.y > 360)
		rot.y = rot.y - 360;
	else if (rot.y < -360)
		rot.y = rot.y + 360;
}
