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
		virtual void draw() const;

		Player* targetPlayer;
	protected:
		virtual void movePos(float deltaTime);
		virtual void updateMatrix();
		virtual void lookAtPlayer();
};

#endif // ENEMY_HPP
