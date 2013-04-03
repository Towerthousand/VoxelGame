#include "Hitbox.hpp"
#include "World.hpp"

Hitbox::Hitbox(const vec3f& pos, const vec3f& radius) : pos(pos), radius(radius) {
}

Hitbox::~Hitbox() {
}

bool Hitbox::collidesWithWorld(const World &world, vec3f offset) {
	vec3f newPos = pos + offset;
	for(float x = -radius.x; x <= radius.x+0.5; x += 0.5)
		for(float y = -radius.y; y <= radius.y+0.5; y += 0.5)
			for(float z = -radius.z; z <= radius.z+0.5; z += 0.5) {
				vec3f point(std::fmin(x, radius.x),std::fmin(y, radius.y),std::fmin(z, radius.z));
				if(pointCollidesWithWorld(world,point+newPos))
					return true;
			}
	return false;
}

bool Hitbox::collidesWithHitbox(const Hitbox &hitbox, vec3f offset) {
	return false; //TODO
}

bool Hitbox::pointCollidesWithWorld(const World &world, const vec3f& point) {
	if(world.getCubeAbs(floor(point.x),floor(point.y),floor(point.z)).ID != 0)
		return true;
	return false;
}
