#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Camera.hpp"
#include "Entity.hpp"

class Player : public Entity , public Camera {
	public:
		enum { //frustrum planes
			TOP = 0,
			BOTTOM,
			LEFT,
			RIGHT,
			NEAR,
			FAR
		};
		Player(SceneMain* scene, const vec3f &pos, const vec3f &scale = vec3f(1,1,1)); //Player is bind to a world on creation, and will use it for collisions.
		~Player();
		void update(float deltaTime);
		void draw() const;

		void drawFrustum() const;
		bool insideFrustum(const vec3f &center, float radius) const; //sphere-plane check for frustum culling

		unsigned char selectedID; //current blockID, used to place blocks
		bool onFloor;
		bool isJumping;
	private:
		void updateCamera();
		void makeFrustum();
		std::vector<std::vector<vec3f> > frustumPlanes;

};

#endif // PLAYER_HPP
