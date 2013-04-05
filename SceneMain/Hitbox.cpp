#include "Hitbox.hpp"
#include "World.hpp"
#include "GameObject.hpp"

Hitbox::Hitbox(GameObject& parent, hitboxType type, vec3f radius) : radius(radius), type(type), parent(parent) {
}

Hitbox::~Hitbox() {
}

bool Hitbox::collidesWithWorld(vec3f offset) {
	switch (type) {
		case (BOX): {
			vec3f newPos = parent.pos + offset;
			for(float x = -radius.x; x <= radius.x+0.5; x += 0.5)
				for(float y = -radius.y; y <= radius.y+0.5; y += 0.5)
					for(float z = -radius.z; z <= radius.z+0.5; z += 0.5) {
						vec3f point(std::fmin(x, radius.x),std::fmin(y, radius.y),std::fmin(z, radius.z));
						if(pointCollidesWithWorld(point+newPos))
							return true;
					}
			break;
		}
		case (POINT):
			return pointCollidesWithWorld(parent.pos+offset);
			break;
		default:
			break;
	}
	return false;
}

bool Hitbox::collidesWithHitbox(const Hitbox &hitbox, vec3f offset) {
	return false; //TODO
}

bool Hitbox::pointCollidesWithWorld(const vec3f& point) {
	if(parent.parentWorld.getCubeAbs(floor(point.x),floor(point.y),floor(point.z)).ID != 0)
		return true;
	return false;
}
