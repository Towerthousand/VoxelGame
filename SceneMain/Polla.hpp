#ifndef POLLA_HPP
#define POLLA_HPP
#include "Entity.hpp"
#include "Model.hpp"

class Player;
class Polla : public Entity {
	public:
		Polla(World &world, vec3f pos, vec3f scale, Player& player);
		~Polla();

		void update(float deltaTime);
		void draw();
		void movePos(float deltaTime);

		Player& followedPlayer;
		static Model model;
};

#endif // POLLA_HPP
