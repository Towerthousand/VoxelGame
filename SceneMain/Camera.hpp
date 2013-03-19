#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "tools.hpp"

class Camera {
	public:
		Camera();
		~Camera();
		void rotateX(float deg);
		void rotateY(float deg);

		vec2f camRot;
};

#endif // CAMERA_HPP
