#ifndef ARROW_H
#define ARROW_H
#include "../Entity.hpp"
#include "../../Model.hpp"

class Player;
class Arrow : public Entity {
	public:
		Arrow(SceneMain* scene, const vec3f &pos, const vec3f &scale = vec3f(0.08,0.08,0.08));
		~Arrow();

		void update(float deltaTime);
		void draw() const;
		void movePos(float deltaTime);

		static Model model;
};

#endif // ARROW_H
