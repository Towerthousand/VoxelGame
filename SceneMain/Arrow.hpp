#ifndef ARROW_H
#define ARROW_H
#include "Entity.hpp"
#include "Model.hpp"

class Arrow : public Entity {
	public:
		Arrow(World &world, vec3f pos, vec3f scale);
		~Arrow();

		void update(float deltaTime);
		void draw();
		void movePos(float deltaTime);

		static Model model;
};

#endif // ARROW_H