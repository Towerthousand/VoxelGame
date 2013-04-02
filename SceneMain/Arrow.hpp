#ifndef ARROW_H
#define ARROW_H
#include "Entity.hpp"

class Arrow : public Entity {
	public:
		Arrow(World &world);
		~Arrow();

		void update(float deltaTime);
		void draw();
};

#endif // ARROW_H
