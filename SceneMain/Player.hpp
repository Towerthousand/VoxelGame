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
        bool inFrustrum(sf::Vector3f position, float radius); //sphere-plane check for frustrum culling

        sf::Vector3f frustrumPlanes[6];
		sf::Vector3f vel;
        short selectedID;
};

#endif // PLAYER_HPP
