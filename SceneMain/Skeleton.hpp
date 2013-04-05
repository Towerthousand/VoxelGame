#ifndef SKELETON_HPP
#define SKELETON_HPP
#include "Enemy.hpp"

class Skeleton : public Enemy {
	public:
		Skeleton(World& world, vec3f pos, Player& targetPlayer);
		~Skeleton();

		void update(float deltaTime);
		void draw();

		static Model model;
};

#endif // SKELETON_HPP
