#include "SceneMain.hpp"
#include "Game.hpp"

SceneMain::SceneMain(Game &parent) : Scene(parent) {
}

SceneMain::~SceneMain() {
}

bool SceneMain::loadResources() {
	if(!parent.resources.loadTexture("boxTexture","Resources/boxTextures.png"))
		return false;
	return true;
}

bool SceneMain::init() {
	std::cout << "* Loading new scene: Main" << std::endl;

	if (!loadResources()) 
		return false;

	mouse.setPosition(sf::Vector2i(SCRWIDTH/2,SCRHEIGHT/2),parent.getWindow());

	WORLDSEED = rand()%1000;

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(1.0/64.0f,1.0/64.0f,1); //now textures are in pixel coords
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, float(SCRWIDTH)/float(SCRHEIGHT), 0.01f, 10000.0f);

	std::cout << "* Loading chunks" << std::endl;
	for (int x = 0; x < WORLDSIZE; ++x) {
		for (int z = 0; z < WORLDSIZE; ++z) {
			world.regenChunk(x,z,WORLDSEED);
		}
	}
	std::cout << "* Init was succesful" << std::endl;
	return true;
}

void SceneMain::update(float deltaTime) {
	world.update(deltaTime);
}

void SceneMain::draw() {
	//Move matrix to position (according to player)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(Player.rotX, 1, 0, 0);
	glRotatef(Player.rotY, 0, 1, 0);
	glTranslatef(-Player.posX, -Player.posY, -Player.posZ);

	parent.resources.textureBank["boxTexture"]->bind();
	glMatrixMode(GL_MODELVIEW);

	world.draw();
	Player.draw();

	glFlush();
}

void SceneMain::onKeyPressed(float deltaTime) {
	
	//standard WASD, no flying
	GLfloat m[16]; //pointer
	const float vel    = 10.0f;
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		Player.moveZ(m,-vel * deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		Player.moveZ(m,vel * deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		Player.moveX(m,-vel * deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		Player.moveX(m,vel * deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		Player.moveY(m,vel * deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		Player.moveY(m,-vel * deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		parent.onClose();
}

void SceneMain::onMouseButtonPressed(float deltaTime) {

	//Move camera according to mouse imput
	sf::Vector2i mousePos = mouse.getPosition(parent.getWindow());
	if (mousePos.x != SCRHEIGHT/2 || mousePos.y != SCRWIDTH/2){
		Player.rotateX(((float)mousePos.y - SCRHEIGHT/2));
		Player.rotateY(((float)mousePos.x - SCRWIDTH/2));
		mouse.setPosition(sf::Vector2i(SCRWIDTH/2, SCRHEIGHT/2),parent.getWindow());
	}
}

void SceneMain::onClose() {
	std::cout << "* Closing scene: Main" << std::endl;
	parent.resources.deleteTexture("boxTexture");
}

