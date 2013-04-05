#ifndef ENEMY_HPP
#define ENEMY_HPP
#include "Entity.hpp"
#include "Model.hpp"

class Player;
class Enemy : public Entity {
	public:
		Enemy(World& world, vec3f pos, Player& targetPplayer);
		virtual ~Enemy();

		virtual void update(float deltaTime);
		virtual void draw();
		virtual void movePos(float deltaTime);

		virtual void lookAtPlayer();

		Player& targetPlayer;
};

#endif // ENEMY_HPP
