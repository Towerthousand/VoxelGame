#include "SceneMain.hpp"
#include "Game.hpp"
#include "Chunk.hpp"

SceneMain::SceneMain(Game &parent) :
	Scene(parent), WORLDSEED(std::time(0)%1000), player(world),
	debugCounter(0.0), fpsCount(0) {
}

SceneMain::~SceneMain() {
}

bool SceneMain::loadResources() {
	if(!parent.textures().loadTexture("lolwtf","resources/lolwtf.png"))
		return false;
	if(!parent.audio().loadMusic("troll","resources/troll.ogg"))
		return false;
	return true;
}

bool SceneMain::init() {
	outLog("* Loading new scene: Main" );

	if (!loadResources())
		return false;
	//Init music
	//parent.audio().musicBank["troll"]->getTrack().play();
	//parent.audio().musicBank["troll"]->getTrack().setLoop(true);
	//Init debug tags
	parent.font().makeText("Jumping","",20,vec2f(10,190),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("OnFloor","",20,vec2f(10,170),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("FPS","",20,vec2f(10,150),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("Updates","",20,vec2f(10,130),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("Chunks","",20,vec2f(10,110),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("posX","",20,vec2f(10,10),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("posY","",20,vec2f(10,30),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("posZ","",20,vec2f(10,50),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("rotX","",20,vec2f(10,90),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("rotY","",20,vec2f(10,70),sf::Color::White,sf::Text::Bold,false);
	//Set up textures
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(1.0/512.0f,1.0/512.0f,1); //now textures are in pixel coords (only works for world texture)
	//Load world from file
	outLog("* Loading chunks" );
	if (!world.loadDirbaio("resources/out.bin"))
		return false;
	//Center mouse
	mouse.setPosition(vec2i(SCRWIDTH/2,SCRHEIGHT/2),parent.getWindow());
	//Enable lights
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);
	GLfloat diffuse[] = {0.3, 0.3, 0.3, 1.f};
	GLfloat ambient[] = {0.2, 0.2, 0.2, 1.f};
	GLfloat specular[] = {0.1, 0.1, 0.1, 1.f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	outLog("* Init was succesful" );
	return true;
}

void SceneMain::update(float deltaTime) {
	++fpsCount;
	debugCounter += deltaTime;
	if (debugCounter > 1) {
		parent.font().getText("FPS").setString("FPS: " + toString(fpsCount));
		parent.font().getText("Updates").setString("Updates: " + toString(1337));
		debugCounter -= 1;
		fpsCount = 0;
	}
	world.update(deltaTime,player);
	world.traceView(player,5);
	player.update(deltaTime);
}

void SceneMain::draw() const {
	//Move matrix to position (according to player)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(player.camRot.x, 1, 0, 0);
	glRotatef(player.camRot.y, 0, 1, 0);
	glTranslatef(-player.camPos.x, -player.camPos.y, -player.camPos.z);

	//Draw global lights
	GLfloat lightpos0[] = {-0.5, 0.7 , -0.3, 0.};
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos0);
	GLfloat lightpos1[] = {0.4, 0.3 , 0.9, 0.};
	glLightfv(GL_LIGHT1, GL_POSITION, lightpos1);

	parent.textures().useTexture("lolwtf");
	world.draw();
	player.draw();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glFlush();

	//Debug tags
	if (player.isJumping)
		parent.font().getText("Jumping").setString("Jumping: True");
	else
		parent.font().getText("Jumping").setString("Jumping: False");
	if (player.onFloor)
		parent.font().getText("OnFloor").setString("OnFloor: True");
	else
		parent.font().getText("OnFloor").setString("OnFloor: False");
	parent.font().getText("Chunks").setString("Chunks drawn: " + toString(world.chunksDrawn));
	parent.font().getText("posX").setString("X: " + toString(player.pos.x));
	parent.font().getText("posY").setString("Y: " + toString(player.pos.y));
	parent.font().getText("posZ").setString("Z: " + toString(player.pos.z));
	parent.font().getText("rotY").setString("Rot Y: " + toString(player.camRot.y));
	parent.font().getText("rotX").setString("Rot X: " + toString(player.camRot.x));
	//SFML draws (until window.popGLStates())
	glDisable(GL_CULL_FACE);
	parent.getWindow().pushGLStates();
	parent.getWindow().draw(parent.font().getText("Jumping"));
	parent.getWindow().draw(parent.font().getText("OnFloor"));
	parent.getWindow().draw(parent.font().getText("FPS"));
	parent.getWindow().draw(parent.font().getText("Updates"));
	parent.getWindow().draw(parent.font().getText("Chunks"));
	parent.getWindow().draw(parent.font().getText("posX"));
	parent.getWindow().draw(parent.font().getText("posY"));
	parent.getWindow().draw(parent.font().getText("posZ"));
	parent.getWindow().draw(parent.font().getText("rotX"));
	parent.getWindow().draw(parent.font().getText("rotY"));
	parent.getWindow().popGLStates();
	glEnable(GL_CULL_FACE);


}

void SceneMain::onKeyPressed(float deltaTime, const sf::Keyboard::Key& key) {
	switch(key) {
		case sf::Keyboard::E: {
			vec3f newPos;
			newPos.x = rand()%(WORLDWIDTH*CHUNKSIZE);
			newPos.z = rand()%(WORLDWIDTH*CHUNKSIZE);
			newPos.y = world.getSkylightLevel(newPos.x,newPos.z) + 1 + 1.7;
			player.pos = newPos + vec3f(0.5,0,0.5);
			break;
		}
		case sf::Keyboard::Num1:
			player.selectedID = 1;
			break;
		case sf::Keyboard::Num2:
			player.selectedID = 2;
			break;
		case sf::Keyboard::Num3:
			player.selectedID = 3;
			break;
		case sf::Keyboard::Num4:
			player.selectedID = 4;
			break;
		case sf::Keyboard::Num5:
			player.selectedID = 5;
			break;
		case sf::Keyboard::Num6:
			player.selectedID = 6;
			break;
		case sf::Keyboard::Num7:
			player.selectedID = 7;
			break;
		case sf::Keyboard::Num8:
			player.selectedID = 8;
			break;
		case sf::Keyboard::Escape:
			parent.close();
			break;
		default:
			break;
	}
}

void SceneMain::onKeyDown(float deltaTime, const sf::Keyboard::Key &key) {
	//Move player
	const float vel = 5.0f;
	vec2f dir(sin(player.camRot.y*DEG_TO_RAD), -cos(player.camRot.y*DEG_TO_RAD));
	switch(key) {
		case sf::Keyboard::W:
			player.vel.x += dir.x*vel;
			player.vel.z += dir.y*vel;
			break;
		case sf::Keyboard::S:
			player.vel.x += -dir.x*vel;
			player.vel.z += -dir.y*vel;
			break;
		case sf::Keyboard::A:
			player.vel.x += dir.y*vel;
			player.vel.z += -dir.x*vel;
			break;
		case sf::Keyboard::D:
			player.vel.x += -dir.y*vel;
			player.vel.z += dir.x*vel;
			break;
		case sf::Keyboard::Space:
			if (player.onFloor && !player.isJumping)
				player.vel.y = 10;
			break;
		default:
			break;
	}
}

void SceneMain::onKeyReleased(float deltaTime, const sf::Keyboard::Key &key) {
	switch(key) {
		default:
			break;
	}
}

void SceneMain::onMouseButtonPressed(float deltaTime, const sf::Mouse::Button& button) {
	switch(button) {
		case sf::Mouse::Left: //delete block
			if(world.playerTargetsBlock) {
				world.setCubeIDAbs(world.targetedBlock.x,world.targetedBlock.y,world.targetedBlock.z,0);
			}
			break;
		case sf::Mouse::Right: //place block
			if(world.playerTargetsBlock) {
				world.setCubeIDAbs(world.last.x,world.last.y,world.last.z,player.selectedID);
			}
			break;
		default:
			break;
	}
}

void SceneMain::onMouseButtonDown(float deltaTime, const sf::Mouse::Button& button) {
	switch(button) {
		default:
			break;
	}
}

void SceneMain::onMouseButtonReleased(float deltaTime, const sf::Mouse::Button& button) {
	switch(button) {
		default:
			break;
	}
}

void SceneMain::onMouseMoved(float deltaTime, int dx, int dy) {
	if (parent.input().focus){
		player.rotateX(dy*0.5);
		player.rotateY(dx*0.5);
		mouse.setPosition(vec2i(SCRWIDTH/2, SCRHEIGHT/2),parent.getWindow());
		parent.input().setMousePos(SCRWIDTH/2, SCRHEIGHT/2);
	}
}

void SceneMain::onClose() {
	outLog("* Closing scene: Main" );

	parent.textures().deleteTexture("lolwtf");
	parent.audio().deleteMusic("troll");
}
