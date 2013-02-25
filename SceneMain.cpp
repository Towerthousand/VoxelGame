#include "SceneMain.hpp"
#include "Game.hpp"

SceneMain::SceneMain(Game &parent) : Scene(parent), player(world), WORLDSEED(std::time(0)%1000){
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

	parent.resources.makeText("posX","",20,sf::Vector2f(10,10),sf::Color::White,sf::Text::Bold,false);
	parent.resources.makeText("posY","",20,sf::Vector2f(10,30),sf::Color::White,sf::Text::Bold,false);
	parent.resources.makeText("posZ","",20,sf::Vector2f(10,50),sf::Color::White,sf::Text::Bold,false);
	parent.resources.makeText("rotX","",20,sf::Vector2f(10,90),sf::Color::White,sf::Text::Bold,false);
	parent.resources.makeText("rotY","",20,sf::Vector2f(10,70),sf::Color::White,sf::Text::Bold,false);
	mouse.setPosition(sf::Vector2i(SCRWIDTH/2,SCRHEIGHT/2),parent.getWindow());
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(1.0/64.0f,1.0/64.0f,1); //now textures are in pixel coords
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, float(SCRWIDTH)/float(SCRHEIGHT), 0.01f, 500.0f);

	std::cout << "* Loading chunks" << std::endl;
	for (int x = 0; x < WORLDSIZE; ++x) {
		for (int z = 0; z < WORLDSIZE; ++z) {
			world.regenChunk(x,z,WORLDSEED);
		}
	}

	std::cout << "* Init was succesful" << std::endl;
	return true;
}

void SceneMain::update(const float &deltaTime) {
	player.vel = sf::Vector3f(0, 0, 0);
	world.update(deltaTime);
	world.traceView(player,50);
	//Constant input (done every frame)
	//Rotate camera according to mouse input
	sf::Vector2i mousePos = mouse.getPosition(parent.getWindow());//sf::Vector2i(event.mouseMove.x,event.mouseMove.y);
	if ((mousePos.x != SCRHEIGHT/2 || mousePos.y != SCRWIDTH/2) && WINDOWFOCUS){
		player.rotateX(((float)mousePos.y - SCRHEIGHT/2));
		player.rotateY(((float)mousePos.x - SCRWIDTH/2));
		mouse.setPosition(sf::Vector2i(SCRWIDTH/2, SCRHEIGHT/2),parent.getWindow());
	}
	//Move player
	const float vel = 10.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		player.vel.z += 20;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		player.vel.z -= 20;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		player.vel.x -= 20;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		player.vel.x += 20;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		player.vel.y += 20;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		player.vel.y -= 20;
	//Looking tests
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7)){
		player.rot.y = 0;
		player.rot.x = 0;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1)){
		player.rot.x = 89.9;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3)){
		player.rot.y = 90;
		player.rot.x = 0;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)){
		player.rot.y = 0;
		player.rot.x = 45;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)){
		player.rot.y = 90;
		player.rot.x = 45;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5)){
		player.rot.y = 45;
		player.rot.x = 45;
	}
	player.update(deltaTime);
}

void SceneMain::draw() const {
	//Move matrix to position (according to player)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(player.rot.x, 1, 0, 0);
	glRotatef(player.rot.y, 0, 1, 0);
	glTranslatef(-player.pos.x, -player.pos.y, -player.pos.z);
	parent.resources.bindTexture("boxTexture");
	world.draw();
	player.draw();
	glFlush();

	std::stringstream ss;
	ss << "X: " << player.pos.x;
	parent.resources.texts["posX"]->setString(ss.str());
	ss.str("");
	ss << "Y: " << player.pos.y;
	parent.resources.texts["posY"]->setString(ss.str());
	ss.str("");
	ss << "Z: " << player.pos.z;
	parent.resources.texts["posZ"]->setString(ss.str());
	ss.str("");
	ss << "Rot Y: " << player.rot.y;
	parent.resources.texts["rotY"]->setString(ss.str());
	ss.str("");
	ss << "Rot X: " << player.rot.x;
	parent.resources.texts["rotX"]->setString(ss.str());
	ss.str("");
	glDisable(GL_CULL_FACE);
	parent.getWindow().pushGLStates();
	parent.getWindow().draw(*parent.resources.texts["posX"]);
	parent.getWindow().draw(*parent.resources.texts["posY"]);
	parent.getWindow().draw(*parent.resources.texts["posZ"]);
	parent.getWindow().draw(*parent.resources.texts["rotX"]);
	parent.getWindow().draw(*parent.resources.texts["rotY"]);
	parent.getWindow().popGLStates();
	glEnable(GL_CULL_FACE);
}

void SceneMain::onKeyPressed(const float& deltaTime, const sf::Event& event) {
}

void SceneMain::onMouseButtonPressed(const float& deltaTime, const sf::Event& event) {
	switch(event.mouseButton.button) {
		case sf::Mouse::Left:
			if(world.playerTargetsBlock) {
				world.setCubeAbs(world.targetedBlock.x,world.targetedBlock.y,world.targetedBlock.z,0);
			}
			break;
		case sf::Mouse::Right:
			if(world.playerTargetsBlock) {
				world.setCubeAbs(world.last.x,world.last.y,world.last.z,2);
			}
			break;
		default:
			break;
	}
}

void SceneMain::onMouseMoved(const float& deltaTime, const sf::Event& event) {
}

void SceneMain::onClose() {
	std::cout << "* Closing scene: Main" << std::endl;
	parent.resources.deleteTexture("boxTexture");
}
