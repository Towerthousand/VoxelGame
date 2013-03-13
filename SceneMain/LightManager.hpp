#ifndef LIGHTMANAGER_HPP
#define LIGHTMANAGER_HPP
#include "tools.hpp"

class LightManager
{
	public:
		LightManager();
		~LightManager();
		void submitLightBox(sf::Vector3i source, sf::Vector2i radius, bool blockChanged);
		void threadedLight(std::queue<lightBox>& queue, bool& status); //will set status to false when done.
		void update();

		bool isRunning;
		std::queue<lightBox> lightBoxQueue;
		std::thread thread;
		bool isThreadRunning;
};

#endif // LIGHTMANAGER_HPP