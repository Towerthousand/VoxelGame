#ifndef CAMERA_H
#define CAMERA_H
#include "tools.hpp"

class World;
class Camera {
	public:
		Camera();
		~Camera();

		float posX, posY,posZ,
			  rotX, rotY, rotZ;
		void moveX(GLfloat m[16], float distance);
		void moveY(GLfloat m[16], float distance);
		void moveZ(GLfloat m[16], float distance);
		void rotateX(float deg);
		void rotateY(float deg);
		void draw();
		bool collide(const World& world);
};

#endif // _HPP
