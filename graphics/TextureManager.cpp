#include "TextureManager.hpp"

TextureManager::TextureManager() {
}

TextureManager::~TextureManager() {
}

bool TextureManager::loadTexture(const std::string& textureID, const std::string& filePath) {
	if (textureBank.count(textureID) != 0) {
		outLog("#WARNING \"" + textureID + "\" already loaded! Overwriting texture..");
		deleteTexture(textureID);
	}
	outLog("* Loading new texture: \"" + textureID + "\" from " + filePath );
	Texture newTexture;
	if (!newTexture.load(filePath))
		return false;
	textureBank[textureID] = newTexture;
	return true;
}

void TextureManager::useTexture(const std::string& textureID) {
	if (textureBank.count(textureID) == 0) {
		textureBank.erase(textureID);
		outLog("#WARNING Trying to bind unexisting textureID: \"" + textureID + "\"");
	}
	else textureBank[textureID].bind();
}

void TextureManager::deleteTexture(const std::string& textureID) {
	if (textureBank.count(textureID) != 0) {
		textureBank.erase(textureID);
		outLog("* Deleting texture: \"" + textureID + "\"");
	}
	else
		outLog("#WARNING Texture \"" + textureID + "\" doesn't exist! Unable to delete");
}

