#ifndef MEDIAMANAGER_CLASS_H
#define MEDIAMANAGER_CLASS_H
#include "tools.hpp"

class MediaManager {
	public:
		MediaManager();
		~MediaManager();
		bool loadMusic(const std::string& trackName, const std::string& filePath);
		bool loadEffect(const std::string& effectName, const std::string& filePath);
		void deleteMusic(const std::string& trackName);
		void deleteEffect(const std::string& effectName);
		bool makeText(const std::string& textName, const std::string& textString,
					  const int& size, const sf::Vector2f& position,
					  const sf::Color& color, const sf::Text::Style& style, const bool& center_origin);
		void deleteText(const std::string& textName);
		bool loadTexture(const std::string& textureName, const std::string& filePath);
		void deleteTexture(const std::string& textureName);
		void bindTexture(const std::string& textureName);

		sf::Font globalFont;
		std::map<std::string,sf::Text *> texts;
		std::map<std::string,sf::Music *> musicBank;
		std::map<std::string,std::pair<sf::Sound *,sf::SoundBuffer *> > effectBank;
		std::map<std::string,std::pair<GLuint, sf::Image *> > textureBank;
};

#endif
