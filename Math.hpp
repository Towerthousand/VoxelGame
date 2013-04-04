#ifndef MATH_HPP
#define MATH_HPP
#include <SFML/System.hpp>

float dot(const sf::Vector3f& a, const sf::Vector3f& b);
sf::Vector3f cross(const sf::Vector3f& a, const sf::Vector3f& b);
void normalize(sf::Vector3f& x);
float norm(const sf::Vector3f& x);

#endif // MATH_HPP
