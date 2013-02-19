#ifndef MEDIAMANAGER_CLASS_H
#define MEDIAMANAGER_CLASS_H
#include "tools.hpp"

class MediaManager {
	public:
		MediaManager();
		~MediaManager();
		bool loadMusic(std::string trackName, std::string filePath);
		bool loadEffect(std::string effectName, std::string filePath);
		void deleteMusic(std::string trackName);
		void deleteEffect(std::string effectName);
		bool makeText(std::string textName, std::string textString,
					  int size, sf::Vector2f position,
					  sf::Color color, sf::Text::Style style, bool center_origin);
		void deleteText(std::string textName);
		bool loadTexture(std::string textureName, std::string filePath);
		void deleteTexture(std::string textureName);
		void bindTexture(std::string textureName);

		sf::Font globalFont;
		std::map<std::string,sf::Text *> texts;
		std::map<std::string,sf::Music *> musicBank;
		std::map<std::string,std::pair<sf::Sound *,sf::SoundBuffer *> > effectBank;
		std::map<std::string,std::pair<GLuint, sf::Image *> > textureBank;
};
#endif
