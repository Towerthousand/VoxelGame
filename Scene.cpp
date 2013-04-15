#include "Scene.hpp"
#include "Game.hpp"

Scene::Scene(Game &parent) : parent(parent) {}
Scene::~Scene() {}
ShaderManager& Scene::getShaders() { return parent.shaders(); }
InputManager& Scene::getInputs() { return parent.input(); }
TextureManager& Scene::getTextures() { return parent.textures(); }
FontManager& Scene::getFonts() { return parent.font(); }
AudioManager& Scene::getAudios() { return parent.audio(); }
