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
		Player(World &world); //Player is bind to a world on creation, and will use it for collisions.
		~Player();
		void update(float deltaTime);
		void draw() const;
		void drawFrustum() const;
		void makeFrustum();
		bool insideFrustum(vec3f center, float radius) const; //sphere-plane check for frustum culling

		vec3f vel;
		short selectedID; //current blockID, used to place blocks
	private:
		std::vector<std::vector<vec3f> > frustumPlanes;
};

#endif // PLAYER_HPP
