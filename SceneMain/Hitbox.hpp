#ifndef HITBOX_HPP
#define HITBOX_HPP
#include "tools.hpp"

class GameObject;
class Hitbox {
	public:
		enum hitboxType {
			BOX = 0,
			POINT
		};

		Hitbox(GameObject& parent, hitboxType type, vec3f radius = vec3f(0,0,0));
		~Hitbox();

		bool collidesWithWorld(vec3f offset = vec3f(0,0,0));
		bool collidesWithHitbox(const Hitbox& hitbox, vec3f offset = vec3f(0,0,0));

		vec3f radius;
		hitboxType type;

	private:
		bool pointCollidesWithWorld(const vec3f& point);

		GameObject& parent;

		//friend class GameObject;
};

#endif // HITBOX_HPP
