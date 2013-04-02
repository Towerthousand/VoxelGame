#ifndef HITBOX_HPP
#define HITBOX_HPP
#include "tools.hpp"

class World;
class Hitbox
{
	public:
		Hitbox(const vec3f& pos, const vec3f& radius);
		~Hitbox();

		bool collidesWithWorld(const World& world, vec3f offset = vec3f(0,0,0));
		bool collidesWithHitbox(const Hitbox& hitbox, vec3f offset = vec3f(0,0,0));
		bool pointCollidesWithWorld(const World &world, const vec3f& point);

		vec3f pos;
		vec3f radius;
};

#endif // HITBOX_HPP
