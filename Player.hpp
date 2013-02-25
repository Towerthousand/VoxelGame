#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "tools.hpp"
#include "Camera.hpp"

class Player : public Camera {
	public:
		Player(const World &world);
		~Player();
		void update(const float& deltaTime);
		void draw() const;
		void applyVel(const float&deltaTime);

		sf::Vector3f vel;
};

#endif // PLAYER_HPP
