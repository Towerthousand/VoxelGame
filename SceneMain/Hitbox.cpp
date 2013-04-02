#include "Hitbox.hpp"
#include "World.hpp"

Hitbox::Hitbox(const vec3f& pos, const vec3f& radius) : pos(pos), radius(radius) {
}

Hitbox::~Hitbox() {
}

bool Hitbox::collidesWithWorld(const World &world, vec3f offset) {
	//	if(radius.x <= 1 && radius.y <= 1 && radius.z <=1) {
	//		for(int x = -1; x < 2; x += 2)
	//			for(int y = -1; y < 2; y += 2)
	//				for(int z = -1; z < 2; z += 2)
	//					if(pointCollidesWithWorld(world,vec3f(pos.x+radius.x*x,pos.y+radius.y*y,pos.z+radius.z*z)))
	//						return true;
	//	}
	//	else
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
