#include "SceneMain.hpp"
#include "Game.hpp"

SceneMain::SceneMain(Game &parent) : Scene(parent), WORLDSEED(std::time(0)%1000), player(world){
}

SceneMain::~SceneMain() {
}

bool SceneMain::loadResources() {
	if(!parent.textures().loadTexture("lolwtf","resources/lolwtf.png"))
		return false;
	return true;
}

bool SceneMain::init() {
	outLog("* Loading new scene: Main" );

	if (!loadResources())
		return false;

	parent.font().makeText("Chunks","",20,sf::Vector2f(10,110),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("posX","",20,sf::Vector2f(10,10),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("posY","",20,sf::Vector2f(10,30),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("posZ","",20,sf::Vector2f(10,50),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("rotX","",20,sf::Vector2f(10,90),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("rotY","",20,sf::Vector2f(10,70),sf::Color::White,sf::Text::Bold,false);
	mouse.setPosition(sf::Vector2i(SCRWIDTH/2,SCRHEIGHT/2),parent.getWindow());
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(1.0/512.0f,1.0/512.0f,1); //now textures are in pixel coords
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, float(SCRWIDTH)/float(SCRHEIGHT), 0.01f, 500.0f);

	outLog("* Loading chunks" );
	for (int x = 0; x < WORLDSIZE; ++x) {
		for (int z = 0; z < WORLDSIZE; ++z) {
			world.regenChunk(x,z,WORLDSEED);
		}
	}

	outLog("* Init was succesful" );
	return true;
}

void SceneMain::update(float deltaTime) {
	player.vel = sf::Vector3f(0, 0, 0);
	world.update(deltaTime,player);
	world.traceView(player,5);
	//Constant input (done every frame)
	//Rotate camera according to mouse input
	sf::Vector2i mousePos = mouse.getPosition(parent.getWindow());//sf::Vector2i(event.mouseMove.x,event.mouseMove.y);
	if ((mousePos.x != SCRHEIGHT/2 || mousePos.y != SCRWIDTH/2) && WINDOWFOCUS){
		player.rotateX(((float)mousePos.y - SCRHEIGHT/2));
		player.rotateY(((float)mousePos.x - SCRWIDTH/2));
		mouse.setPosition(sf::Vector2i(SCRWIDTH/2, SCRHEIGHT/2),parent.getWindow());
	}
	//Move player
	const float vel = 5.0f;
	sf::Vector2f dir(-sin(-player.rot.y*DEG_TO_RAD), -cos(player.rot.y*DEG_TO_RAD));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		player.vel.x += dir.x*vel*deltaTime;
		player.vel.z += dir.y*vel*deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		player.vel.x += -dir.x*vel*deltaTime;
		player.vel.z += -dir.y*vel*deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		player.vel.x += dir.y*vel*deltaTime;
		player.vel.z += -dir.x*vel*deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player.vel.x += -dir.y*vel*deltaTime;
		player.vel.z += dir.x*vel*deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		player.vel.y += 4*vel*deltaTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		player.vel.y -= vel*deltaTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		player.pos = sf::Vector3f(0,70,0);

	player.update(deltaTime);
}

void SceneMain::draw() const {
	//Move matrix to position (according to player)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(player.rot.x, 1, 0, 0);
	glRotatef(player.rot.y, 0, 1, 0);
	glTranslatef(-player.pos.x, -player.pos.y, -player.pos.z);

	parent.textures().useTexture("lolwtf");
	world.draw();
	player.draw();
	glFlush();

	//Debug tags
	parent.font().getText("Chunks").setString("Chunks drawn: " + toString(world.chunksDrawn));
	parent.font().getText("posX").setString("X: " + toString(player.pos.x));
	parent.font().getText("posY").setString("Y: " + toString(player.pos.y));
	parent.font().getText("posZ").setString("Z: " + toString(player.pos.z));
	parent.font().getText("rotY").setString("Rot Y: " + toString(player.rot.y));
	parent.font().getText("rotX").setString("Rot X: " + toString(player.rot.x));
	glDisable(GL_CULL_FACE);
	parent.getWindow().pushGLStates();
	//SFML draws (until window.popGLStates())
	parent.getWindow().draw(parent.font().getText("Chunks"));
	parent.getWindow().draw(parent.font().getText("posX"));
	parent.getWindow().draw(parent.font().getText("posY"));
	parent.getWindow().draw(parent.font().getText("posZ"));
	parent.getWindow().draw(parent.font().getText("rotX"));
	parent.getWindow().draw(parent.font().getText("rotY"));
	parent.getWindow().popGLStates();
	glEnable(GL_CULL_FACE);
}

void SceneMain::onKeyPressed(float deltaTime, const sf::Event& event) {
}

void SceneMain::onMouseButtonPressed(float deltaTime, const sf::Event& event) {
	switch(event.mouseButton.button) {
		case sf::Mouse::Left:
			if(world.playerTargetsBlock)
                world.setCubeIDAbs(world.targetedBlock.x,world.targetedBlock.y,world.targetedBlock.z,0);
			break;
		case sf::Mouse::Right:
			if(world.playerTargetsBlock)
                world.setCubeIDAbs(world.last.x,world.last.y,world.last.z,2);
			break;
		default:
			break;
	}
}

void SceneMain::onMouseMoved(float deltaTime, const sf::Event& event) {
}

void SceneMain::onClose() {
	outLog("* Closing scene: Main" );
	parent.textures().deleteTexture("lolwtf");
}
