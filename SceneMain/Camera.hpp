#ifndef CAMERA_H
#define CAMERA_H
#include "tools.hpp"

class World;
class Camera {
	public:
		Camera(World &world);
		~Camera();
		void movePos(const sf::Vector3f& disp);
		void rotateX(float deg);
		void rotateY(float deg);

		sf::Vector3f pos;
		sf::Vector3f rot;
		World& parentWorld;
};

#endif // CAMERA_HPP
