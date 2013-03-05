#ifndef CAMERA_H
#define CAMERA_H
#include "tools.hpp"

class World;
class Camera {
	public:
		Camera(const World &world);
		~Camera();
		void moveX(float distance);
		void moveY(float distance);
		void moveZ(float distance);
		void rotateX(float deg);
		void rotateY(float deg);

		sf::Vector3f pos;
		sf::Vector3f rot;
		const World& parentWorld;
};

#endif // CAMERA_HPP
