#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "tools.hpp"

class Camera { //Just rotates in a human-like manner. Thought to be used with GameObjects and derived types
	public:
		Camera();
		~Camera();
		void rotateX(float deg);
		void rotateY(float deg);

		vec2f camRot;
		vec3f camPos;
};

#endif // CAMERA_HPP
