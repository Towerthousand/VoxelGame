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
	gluPerspective(60.0f, float(SCRWIDTH)/float(SCRHEIGHT), 0.01f, 100.0f);

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
	world.traceView(player);
}

void SceneMain::draw() {
	//Move matrix to position (according to player)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(player.rotX, 1, 0, 0);
	glRotatef(player.rotY, 0, 1, 0);
	glTranslatef(-player.posX, -player.posY, -player.posZ);

	parent.resources.bindTexture("boxTexture");
	world.draw();
	player.draw();

	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBegin(GL_LINES);
	glColor3f(1,1,1);
	glVertex3f(-0.01,    0,-0.5);
	glVertex3f( 0.01,    0,-0.5);
	glVertex3f(    0,-0.01,-0.5);
	glVertex3f(    0, 0.01,-0.5);
	glEnd();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();

	glFlush();
}

void SceneMain::onKeyPressed(float deltaTime) {
	//standard WASD, no flying
	GLfloat m[16]; //pointer
	const float vel    = 10.0f;
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		player.moveZ(m,-vel * deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		player.moveZ(m,vel * deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		player.moveX(m,-vel * deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		player.moveX(m,vel * deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		player.moveY(m,vel * deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		player.moveY(m,-vel * deltaTime);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		parent.onClose();
}

void SceneMain::onMouseButtonPressed(float deltaTime) {
	//Move camera according to mouse input
	sf::Vector2i mousePos = mouse.getPosition(parent.getWindow());
	if ((mousePos.x != SCRHEIGHT/2 || mousePos.y != SCRWIDTH/2) && WINDOWFOCUS){
		player.rotateX(((float)mousePos.y - SCRHEIGHT/2));
		player.rotateY(((float)mousePos.x - SCRWIDTH/2));
		mouse.setPosition(sf::Vector2i(SCRWIDTH/2, SCRHEIGHT/2),parent.getWindow());
	}
	if(mouse.isButtonPressed(sf::Mouse::Left) && world.playerTargetsBlock) {
		world.setCubeAbs(world.targetedBlock.x,world.targetedBlock.y,world.targetedBlock.z,0);
	}
	if(mouse.isButtonPressed(sf::Mouse::Right) && world.playerTargetsBlock) {
		world.setCubeAbs(world.last.x,world.last.y,world.last.z,2);
	}
	sf::Event event;
	while (parent.getWindow().pollEvent(event)){
		switch(event.type) {
			case sf::Event::MouseButtonPressed:
				break;
			default:
				break;
		}
	}
}

void SceneMain::onClose() {
	std::cout << "* Closing scene: Main" << std::endl;
	parent.resources.deleteTexture("boxTexture");
}

