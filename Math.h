#ifndef MATH_H
#define MATH_H

inline float dot(sf::Vector3f a, sf::Vector3f b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline sf::Vector3f cross(sf::Vector3f a, sf::Vector3f b) {
    return sf::Vector3f(a.y*b.z - b.y*a.z,
                        -a.x*b.z + b.x*a.z,
                        a.x*b.y - b.x*a.y);
}

inline void normalize(sf::Vector3f& x) {
    x = x/float(sqrt(dot(x,x)));
}

#endif // MATH_H
