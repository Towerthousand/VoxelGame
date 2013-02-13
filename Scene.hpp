#ifndef SCENE_CLASS
#define SCENE_CLASS

class Game;
class Scene {
	public:
		Scene(Game &parent) : parent(parent) {} //should NOT do anything apart from being called
		virtual ~Scene() {}
		virtual bool init() = 0; //should call loadResources()
		virtual void update(float deltaTime) = 0; //called by parent on parent.update()
		virtual void draw() = 0; //called by parent on parent.draw()
		virtual void onKeyPressed(float deltaTime) = 0; //called by parent on parent.update()
		virtual void onMouseButtonPressed(float deltaTime) = 0; //called by parent on parent.update()
		virtual void onClose() = 0; //close scene-wide stuff
		
	protected:
		Game& parent;
		virtual bool loadResources() = 0; //load stuff with parent.resources
};

#endif
