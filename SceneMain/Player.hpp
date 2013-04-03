#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Camera.hpp"
#include "Entity.hpp"

class Player : public Entity , public Camera {
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

		short selectedID; //current blockID, used to place blocks
		bool onFloor;
		bool isJumping;
	private:
		std::vector<std::vector<vec3f> > frustumPlanes;

};

#endif // PLAYER_HPP
