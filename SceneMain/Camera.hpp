#ifndef CAMERA_H
#define CAMERA_H
#include "tools.hpp"

class World;
class Camera {
	public:
		Camera(World& world); //Player is bind to a world on creation, and will use it for collisions.
		~Camera();
		void rotateX(float deg);
		void rotateY(float deg);
};

#endif // CAMERA_HPP
