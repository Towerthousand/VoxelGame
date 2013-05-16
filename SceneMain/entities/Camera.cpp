#include "Camera.hpp"
#include "../world/World.hpp"

//
// This implementation of "camera" has no z rotation (fuck quaternions)
// and is thougt of as a no-fly, fps kind of view. Tweak it!
//

Camera::Camera() : camRot(0,0), camPos(0,0,0), viewMatrix(mat4f(1.0)) {
}

Camera::~Camera() {
}

void Camera::rotateX(float deg) {
	camRot.x += deg;

	//Euler fix
	if (camRot.x > 89.9)
		camRot.x = 89.9;
	else if (camRot.x < -89.9)
		camRot.x = -89.9;
}

void Camera::rotateY(float deg) {
	camRot.y += deg;

	//prevent overflow of float rot.y
	if (camRot.y > 360)
		camRot.y = camRot.y - 360;
	else if (camRot.y < -360)
		camRot.y = camRot.y + 360;
}
