#include "Math.hpp"

float dot(const sf::Vector3f& a, const sf::Vector3f& b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

sf::Vector3f cross(const sf::Vector3f& a, const sf::Vector3f& b) {
	return sf::Vector3f(a.y*b.z - b.y*a.z,
						-a.x*b.z + b.x*a.z,
						a.x*b.y - b.x*a.y);
}

void normalize(sf::Vector3f& x) {
	x = x/float(sqrt(dot(x,x)));
}

float norm(const sf::Vector3f& x) {
	return float(sqrt(dot(x,x)));
}
