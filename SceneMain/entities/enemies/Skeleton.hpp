#ifndef SKELETON_HPP
#define SKELETON_HPP
#include "Enemy.hpp"

class Skeleton : public Enemy {
	public:
		Skeleton(SceneMain* world, const vec3f &pos, Player* targetPlayer, const vec3f &scale = vec3f(0.2,0.2,0.2));
		~Skeleton();

		void update(float deltaTime);
		void draw() const;

		static Model model;
	private:
		void updateMatrix();

		float cooldown;
		vec3f shootPosOffset;//where does the arrow exit from (offset from pos)
		float ambientLight;
};

#endif // SKELETON_HPP
