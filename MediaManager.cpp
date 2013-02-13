#include "MediaManager.hpp"

MediaManager::MediaManager() {
}

MediaManager::~MediaManager() {
}

bool MediaManager::loadMusic(std::string trackName, std::string filePath) {
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

bool MediaManager::loadEffect(std::string effectName, std::string filePath) {
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

void MediaManager::deleteMusic(std::string trackName) {
	if (musicBank.count(trackName) != 0)
		if (musicBank[trackName] != NULL) {
			std::cout << "* Deleting music track \"" << trackName << "\"" << std::endl;
			delete musicBank[trackName];
			musicBank[trackName] = NULL;
		}
}

void MediaManager::deleteEffect(std::string effectName) {
	if (effectBank.count(effectName) != 0)
		if (effectBank[effectName].first != NULL) {
			std::cout << "* Deleting sound effect \"" << effectName << "\"" << std::endl;
			delete effectBank[effectName].first;
			delete effectBank[effectName].second;
			effectBank[effectName].first = NULL;
			effectBank[effectName].second = NULL;
		}
}

bool MediaManager::makeText(std::string textName, std::string textString,
							int size, sf::Vector2f position,
							sf::Color color = sf::Color::White,
							sf::Text::Style style = sf::Text::Regular,
							bool center_origin = false) {
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

void MediaManager::deleteText(std::string textName) {
	if (texts.count(textName) != 0)
		if (texts[textName] != NULL) {
			std::cout << "* Deleting text \"" << textName << "\"" << std::endl;
			delete texts[textName];
			texts[textName] = NULL;
		}
}

bool MediaManager::loadTexture(std::string textureName, std::string filePath) {
	if (textureBank.count(textureName) != 0)
		if (textureBank[textureName] != NULL) {
			std::cout << "#WARNING " << textureName << " already loaded!"<< std::endl;
			return true;
		}
	std::cout << "* Loading new texture: \"" << textureName << "\" from " << filePath << std::endl;
	sf::Texture * newTexture = new sf::Texture;
	if (!newTexture->loadFromFile(filePath)) {
		std::cout << "#ERROR " << filePath << " didn't load"<< std::endl;
		return false;
	}
	newTexture->setSmooth(false);
	textureBank[textureName] = newTexture;
	return true;
}

void MediaManager::deleteTexture(std::string textureName) {
	if (textureBank.count(textureName) != 0)
		if (textureBank[textureName] != NULL) {
			delete textureBank[textureName];
			textureBank[textureName] = NULL;
			std::cout << "* Deleting texture \"" << textureName << "\"" << std::endl;
		}
}
