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
	parent.font().makeText("LOL","SDADASDAS",20,vec2f(10,170),sf::Color::White,sf::Text::Bold,false);
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
	//Set up GL view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, float(SCRWIDTH)/float(SCRHEIGHT), ZNEAR, ZFAR);
	//Load world from file
	outLog("* Loading chunks" );
	if (!world.loadDirbaio("resources/out.bin"))
		return false;
	//Center mouse
	mouse.setPosition(vec2i(SCRWIDTH/2,SCRHEIGHT/2),parent.getWindow());

	outLog("* Init was succesful" );
	return true;
}

void SceneMain::update(float deltaTime) {
	++fpsCount;
	debugCounter += deltaTime;
	if (debugCounter > 1) {
		parent.font().getText("FPS").setString("FPS: " + toString(fpsCount));
		parent.font().getText("Updates").setString("Updates: " + toString(DBG_UPDATES));
		DBG_UPDATES = 0;
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
	glRotatef(player.rot.x, 1, 0, 0);
	glRotatef(player.rot.y, 0, 1, 0);
	glTranslatef(-player.pos.x, -player.pos.y, -player.pos.z);

	parent.textures().useTexture("lolwtf");
	world.draw();
	player.draw();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glFlush();

	//Debug tags
	parent.font().getText("Chunks").setString("Chunks drawn: " + toString(world.chunksDrawn));
	parent.font().getText("posX").setString("X: " + toString(player.pos.x));
	parent.font().getText("posY").setString("Y: " + toString(player.pos.y));
	parent.font().getText("posZ").setString("Z: " + toString(player.pos.z));
	parent.font().getText("rotY").setString("Rot Y: " + toString(player.rot.y));
	parent.font().getText("rotX").setString("Rot X: " + toString(player.rot.x));
	//SFML draws (until window.popGLStates())
	glDisable(GL_CULL_FACE);
	parent.getWindow().pushGLStates();
	parent.getWindow().draw(parent.font().getText("LOL"));
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
			newPos.y = world.getSkylightLevel(newPos.x,newPos.z) + 1 - PLAYER_HEIGHT;
			parent.font().getText("LOL").setString(toString(world.getSkylightLevel(newPos.x,newPos.z)));
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
	vec2f dir(sin(player.rot.y*DEG_TO_RAD), -cos(player.rot.y*DEG_TO_RAD));
	switch(key) {
		case sf::Keyboard::W:
			player.vel.x += dir.x*vel*deltaTime;
			player.vel.z += dir.y*vel*deltaTime;
			break;
		case sf::Keyboard::S:
			player.vel.x += -dir.x*vel*deltaTime;
			player.vel.z += -dir.y*vel*deltaTime;
			break;
		case sf::Keyboard::A:
			player.vel.x += dir.y*vel*deltaTime;
			player.vel.z += -dir.x*vel*deltaTime;
			break;
		case sf::Keyboard::D:
			player.vel.x += -dir.y*vel*deltaTime;
			player.vel.z += dir.x*vel*deltaTime;
			break;
		case sf::Keyboard::Space:
			player.vel.y += 4*vel*deltaTime;
			break;
		case sf::Keyboard::LShift:
			player.vel.y -= vel*deltaTime;
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

void SceneMain::onMouseMoved(float deltaTime) {
	vec2i mousePos = mouse.getPosition(parent.getWindow());//vec2i(event.mouseMove.x,event.mouseMove.y);
	if ((mousePos.x != SCRHEIGHT/2 || mousePos.y != SCRWIDTH/2) && WINDOWFOCUS){
		player.rotateX(((float)mousePos.y - SCRHEIGHT/2)*0.5);
		player.rotateY(((float)mousePos.x - SCRWIDTH/2)*0.5);
		mouse.setPosition(vec2i(SCRWIDTH/2, SCRHEIGHT/2),parent.getWindow());
	}
}

void SceneMain::onClose() {
	outLog("* Closing scene: Main" );

	parent.textures().deleteTexture("lolwtf");
	parent.audio().deleteMusic("troll");
}
