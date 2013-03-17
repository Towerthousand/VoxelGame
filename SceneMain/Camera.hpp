#ifndef CAMERA_H
#define CAMERA_H
#include "tools.hpp"

class World;
class Camera {
	public:
		Camera(World &world); //Player is bind to a world on creation, and will use it for collisions.
		~Camera();
        void movePos(const vec3f& disp);
		void rotateX(float deg);
		void rotateY(float deg);

        vec3f pos;
		vec3f rot;
		World& parentWorld;
};

#endif // CAMERA_HPP
