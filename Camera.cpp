#include "Camera.hpp"

//
// This implementation of "camera" has no z rotation (fuck quaternions)
// and is thougt of as a no-fly, fps kind of view. Tweak it!
//

Camera::Camera() {
	posX = 0;
	posZ = 0;
	posY = 70;
	rotX = 0.0;
	rotY = 0.0;
	rotZ = 0.0;
}

Camera::~Camera() {
}

void Camera::moveX(GLfloat* m,float distance) {
	posX += m[0] * distance;
	//posY += m[4] * distance; No flying
	posZ += m[8] * distance;
}

void Camera::moveY(GLfloat* m,float distance) {
	posY += distance; // not gonna spread it throuh X and Z to avoid flying.
					  // to spread it use x += m[1], y += m[5], z += m[9]
}

void Camera::moveZ(GLfloat* m,float distance) {
	posX += (m[2]) * (distance);
	//posY += m[6] * distance; I SAID NO FLYING! DAMMIT, HAROLD
	posZ += (m[10]) * (distance);
}

void Camera::rotateX(float deg) {
	rotX += deg;

	//Euler fix
	if (rotX > 89.9)
		rotX = 89.9;
	else if (rotX < -89.9)
		rotX = -89.9;
}

void Camera::rotateY(float deg) {
	rotY += deg;

	//prevent overflow of float rotY
	if (rotY > 360)
		rotY = rotY - 360;
	else if (rotY < -360)
			rotY = rotY + 360;
}

void Camera::draw() {
	//implement player body
}
