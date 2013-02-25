#ifndef CAMERA_H
#define CAMERA_H
#include "tools.hpp"

class World;
class Camera {
	public:
		Camera(const World &world);
		~Camera();
		void moveX(const float& distance);
		void moveY(const float& distance);
		void moveZ(const float& distance);
		void rotateX(const float &deg);
		void rotateY(const float& deg);

		sf::Vector3f pos;
		sf::Vector3f rot;
		const World& parentWorld;
};

#endif // _HPP
