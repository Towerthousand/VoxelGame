#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Camera.hpp"

class Player : public Camera {
	public:
		Player(World &world);
		~Player();
		void update(float deltaTime);
		void draw() const;

		sf::Vector3f vel;
		int selectedID;
};

#endif // PLAYER_HPP
