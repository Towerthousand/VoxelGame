#ifndef SKELETON_HPP
#define SKELETON_HPP
#include "Enemy.hpp"

class Skeleton : public Enemy {
	public:
		Skeleton(SceneMain* world, vec3f pos, vec3f scale, Player* targetPlayer);
		~Skeleton();

		void update(float deltaTime);
		void draw() const;

		static Model model;
		float cooldown;
		vec3f shootPosOffset;//where does the arrow exit from (offset from pos)
};

#endif // SKELETON_HPP
