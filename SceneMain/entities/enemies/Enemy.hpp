#ifndef ENEMY_HPP
#define ENEMY_HPP
#include "../Entity.hpp"
#include "../../Model.hpp"

class Player;
class Enemy : public Entity {
	public:
		Enemy(SceneMain* scene, const vec3f &pos, const vec3f &scale, Player* targetPlayer);
		virtual ~Enemy();

		virtual void update(float deltaTime);
		virtual void updateMatrix();
		virtual void draw() const;
		virtual void movePos(float deltaTime);

		virtual void lookAtPlayer();

		Player* targetPlayer;
};

#endif // ENEMY_HPP
