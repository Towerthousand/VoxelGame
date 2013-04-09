#ifndef POLLA_HPP
#define POLLA_HPP
#include "Entity.hpp"
#include "Model.hpp"

class Player;
class Polla : public Entity {
	public:
		Polla(SceneMain* scene, const vec3f &pos, Player* player, const vec3f &scale = vec3f(0.1,0.1,0.1));
		~Polla();

		void update(float deltaTime);
		void draw() const;
		void movePos(float deltaTime);

		Player* followedPlayer;
		static Model model;
};

#endif // POLLA_HPP
