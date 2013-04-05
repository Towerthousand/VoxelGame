#include "SceneMain.hpp"
#include "Game.hpp"
#include "Chunk.hpp"
#include "Arrow.hpp"
#include "Polla.hpp"
#include "Skeleton.hpp"

SceneMain::SceneMain(Game &parent) :
	Scene(parent), WORLDSEED(std::time(0)%1000), player(world, vec3f(0,256,0)),
	debugCounter(0.0), fpsCount(0) {
}

SceneMain::~SceneMain() {
	for(std::vector<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it) {
		if(*it != NULL)
			delete *it;
	}
}

bool SceneMain::loadResources() {
	if(!parent.textures().loadTexture("lolwtf","resources/lolwtf.png"))
		return false;
	if(!parent.audio().loadMusic("troll","resources/troll.ogg"))
		return false;
	if(!Arrow::model.loadVoxelization("resources/arrow.vox"))
		return false;
	if(!Polla::model.loadVoxelization("resources/pene.vox"))
		return false;
	if(!Arrow::model.loadVoxelization("resources/mob.vox"))
		return false;
	outLog("* Loading chunks" );
	if (!world.loadDirbaio("resources/out.bin"))
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
	parent.font().makeText("posX","",20,vec2f(10,10),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("posY","",20,vec2f(10,30),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("posZ","",20,vec2f(10,50),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("rotY","",20,vec2f(10,70),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("rotX","",20,vec2f(10,90),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("Chunks","",20,vec2f(10,110),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("Updates","",20,vec2f(10,130),sf::Color::White,sf::Text::Bold,false);
	parent.font().makeText("FPS","",20,vec2f(10,150),sf::Color::White,sf::Text::Bold,false);
	//Set up textures
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(1.0/512.0f,1.0/512.0f,1); //now textures are in pixel coords (only works for world texture)
	//Center mouse
	mouse.setPosition(vec2i(SCRWIDTH/2,SCRHEIGHT/2),parent.getWindow());
	//Enable lights
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	GLfloat diffuse[] = {0.3, 0.3, 0.3, 1.f};
	GLfloat ambient[] = {0.2, 0.2, 0.2, 1.f};
	GLfloat specular[] = {0.1, 0.1, 0.1, 1.f};
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);

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
	for(uint i = 0; i < objects.size(); ++i)
		if(objects[i] != NULL) {
			if (std::fabs(norm(objects[i]->pos-player.pos)) > 200) {
				delete objects[i];
				objects[i] = NULL;
			}
			else
				objects[i]->update(deltaTime);
		}
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

	//Draw all the stuff
	for(uint i = 0; i < objects.size(); ++i)
		if(objects[i] != NULL)
			objects[i]->draw();
	parent.textures().useTexture("lolwtf");
	world.draw();
	player.draw();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glFlush();

	//Debug tags
	parent.font().getText("posX").setString("X: " + toString(player.pos.x));
	parent.font().getText("posY").setString("Y: " + toString(player.pos.y));
	parent.font().getText("posZ").setString("Z: " + toString(player.pos.z));
	parent.font().getText("rotY").setString("Rot Y: " + toString(player.camRot.y));
	parent.font().getText("rotX").setString("Rot X: " + toString(player.camRot.x));
	parent.font().getText("Chunks").setString("Chunks drawn: " + toString(world.chunksDrawn));

	//SFML draws (until window.popGLStates())
	glDisable(GL_CULL_FACE);
	parent.getWindow().pushGLStates();
	parent.getWindow().draw(parent.font().getText("posX"));
	parent.getWindow().draw(parent.font().getText("posY"));
	parent.getWindow().draw(parent.font().getText("posZ"));
	parent.getWindow().draw(parent.font().getText("rotY"));
	parent.getWindow().draw(parent.font().getText("rotX"));
	parent.getWindow().draw(parent.font().getText("Chunks"));
	parent.getWindow().draw(parent.font().getText("Updates"));
	parent.getWindow().draw(parent.font().getText("FPS"));
	parent.getWindow().popGLStates();
	glEnable(GL_CULL_FACE);
}

void SceneMain::onKeyPressed(float deltaTime, const sf::Keyboard::Key& key) {
	switch(key) {
		case sf::Keyboard::E: { //new spawnpoint
			vec3f newPos;
			newPos.x = rand()%(WORLDWIDTH*CHUNKSIZE);
			newPos.z = rand()%(WORLDWIDTH*CHUNKSIZE);
			newPos.y = world.getSkylightLevel(newPos.x,newPos.z) + player.hitbox.radius.y;
			player.pos = newPos + vec3f(0.5,1,0.5);
			newPos.x = rand()%(WORLDWIDTH*CHUNKSIZE);
			newPos.z = rand()%(WORLDWIDTH*CHUNKSIZE);
			newPos.y = world.getSkylightLevel(newPos.x,newPos.z) + player.hitbox.radius.y;
			player.pos = newPos + vec3f(0.5,1,0.5);
			Skeleton * ns = new Skeleton(world,vec3f(0,0,0),player, vec3f(0.04,0.04,0.04));
			objects.push_back(ns);
			break;
		}
		case sf::Keyboard::Num1: {
			player.selectedID = 1;
			break;
		}
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
		case sf::Mouse::Middle: { //Arrow!
			float m[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, m);
			vec3f dir(m[2],m[6],m[10]);//same as the player's pov
			Arrow * np = new Arrow(world,player.camPos, vec3f(0.04,0.04,0.04));
			np->vel -= vec3f(dir.x*30.0,dir.y*30.0,dir.z*30.0);
			objects.push_back(np);
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
