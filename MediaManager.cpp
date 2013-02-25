#include "MediaManager.hpp"

MediaManager::MediaManager() {
}

MediaManager::~MediaManager() {
}

bool MediaManager::loadMusic(const std::string &trackName, const std::string &filePath) {
	if (musicBank.count(trackName) != 0)
		if (musicBank[trackName] != NULL) {
			std::cout << "#WARNING " << trackName << " already loaded!"<< std::endl;
			return true;
		}
	std::cout << "* Loading new music track: \"" << trackName << "\" from " << filePath << std::endl;
	sf::Music * newTrack = new sf::Music();
	if (!newTrack->openFromFile(filePath)) {
		std::cout << "#ERROR " << filePath << " didn't load"<< std::endl;
		return false;
	}
	musicBank[trackName] = newTrack;
	return true;
}

bool MediaManager::loadEffect(const std::string &effectName, const std::string &filePath) {
	if (effectBank.count(effectName) != 0)
		if (effectBank[effectName].first != NULL) {
			std::cout << "#WARNING " << effectName << " already loaded!"<< std::endl;
			return true;
		}
	std::cout << "* Loading new sound effect: \"" << effectName << "\" from " << filePath << std::endl;
	sf::SoundBuffer * buff = new sf::SoundBuffer;
	sf::Sound * sound = new sf::Sound;
	if (!buff->loadFromFile(filePath)) {
		std::cout << "#ERROR " << filePath << " didn't load"<< std::endl;
		return false;
	}
	sound->setBuffer(*buff);
	effectBank[effectName] = std::pair<sf::Sound *,sf::SoundBuffer *>(sound,buff);
	return true;
}

void MediaManager::deleteMusic(const std::string &trackName) {
	if (musicBank.count(trackName) != 0)
		if (musicBank[trackName] != NULL) {
			std::cout << "* Deleting music track \"" << trackName << "\"" << std::endl;
			delete musicBank[trackName];
			musicBank[trackName] = NULL;
		}
}

void MediaManager::deleteEffect(const std::string &effectName) {
	if (effectBank.count(effectName) != 0)
		if (effectBank[effectName].first != NULL) {
			std::cout << "* Deleting sound effect \"" << effectName << "\"" << std::endl;
			delete effectBank[effectName].first;
			delete effectBank[effectName].second;
			effectBank[effectName].first = NULL;
			effectBank[effectName].second = NULL;
		}
}

bool MediaManager::makeText(const std::string& textName, const std::string& textString,
							const int& size, const sf::Vector2f& position,
							const sf::Color& color, const sf::Text::Style& style,
							const bool& center_origin) {
	if (texts.count(textName) != 0)
		if (texts[textName] != NULL)
			deleteText(textName);
	sf::Text * text = new sf::Text;
	text->setFont(globalFont);
	text->setString(textString);
	text->setCharacterSize(size);
	text->setColor(color);
	text->setStyle(style);
	if (center_origin)
		text->setOrigin(text->getLocalBounds().width/2,
						text->getLocalBounds().height/2);
	text->setOrigin(text->getOrigin().x + text->getLocalBounds().left, //deviation restored
					text->getOrigin().y + text->getLocalBounds().top); //deviation restored
	text->setPosition(position);
	texts[textName] = text;
	return true;
}

void MediaManager::deleteText(const std::string &textName) {
	if (texts.count(textName) != 0)
		if (texts[textName] != NULL) {
			std::cout << "* Deleting text \"" << textName << "\"" << std::endl;
			delete texts[textName];
			texts[textName] = NULL;
		}
}

bool MediaManager::loadTexture(const std::string &textureName, const std::string &filePath) {
	if (textureBank.count(textureName) != 0)
		if (textureBank[textureName].second != NULL) {
			std::cout << "#WARNING " << textureName << " already loaded!"<< std::endl;
			return true;
		}
	std::cout << "* Loading new texture: \"" << textureName << "\" from " << filePath << std::endl;
	sf::Image * newTexture = new sf::Image;
	if (!newTexture->loadFromFile(filePath)) {
		std::cout << "#ERROR " << filePath << " didn't load"<< std::endl;
		return false;
	}
	GLuint texture_handle;
	glGenTextures(1, &texture_handle);
	textureBank[textureName].second = newTexture;
	textureBank[textureName].first = texture_handle;
	return true;
}

void MediaManager::deleteTexture(const std::string &textureName) {
	if (textureBank.count(textureName) != 0)
		if (textureBank[textureName].second != NULL) {
			glDeleteTextures(1,&textureBank[textureName].first);
			delete textureBank[textureName].second;
			textureBank[textureName].second = NULL;
			std::cout << "* Deleting texture \"" << textureName << "\"" << std::endl;
		}
}

void MediaManager::bindTexture(const std::string &textureName) {
	glBindTexture(GL_TEXTURE_2D, textureBank[textureName].first);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		textureBank[textureName].second->getSize().x, textureBank[textureName].second->getSize().y,
		0,
		GL_RGBA, GL_UNSIGNED_BYTE, textureBank[textureName].second->getPixelsPtr()
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
