#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Camera.hpp"

class Player : public Camera {
	public:

        enum {
            TOP = 0,
            BOTTOM,
            LEFT,
            RIGHT,
            NEAR,
            FAR
        };
		Player(World &world);
		~Player();
		void update(float deltaTime);
		void draw() const;
        bool insideFrustum(vec3f position, float radius); //sphere-plane check for frustum culling

        std::vector<std::vector<vec3f> > frustumPlanes;
        vec3f vel;
        short selectedID;
};

#endif // PLAYER_HPP
