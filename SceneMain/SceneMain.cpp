#include "SceneMain.hpp"
#include "Game.hpp"
#include "world/Chunk.hpp"
#include "entities/Player.hpp"
#include "entities/items/Arrow.hpp"
#include "entities/items/Polla.hpp"
#include "entities/enemies/Skeleton.hpp"

SceneMain::SceneMain(Game &parent) :
	Scene(parent), chunksDrawn(0), WORLDSEED(std::time(0)%1000),
	player(new Player(this, vec3f(0,1,0))), world(this,player),
	debugCounter(0.0), fpsCount(0) {
}

SceneMain::~SceneMain() {
	for(std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it) {
		if(*it != NULL)
			delete *it;
	}
}

bool SceneMain::loadResources() {
	if(!getShaders().loadVertexShader("shaders/vertex.glsl", "VERTEX"))
		return false;
	if(!parent.shaders().loadFragmentShader("shaders/fragment.glsl", "FRAGMENT"))
		return false;
	if(!parent.shaders().makeProgram("VERTEX","FRAGMENT","PROGRAM"))
		return false;
	if(!parent.textures().loadTexture("lolwtf","resources/blocks" + toString(TEXSIZE) +".png"))
		return false;
	if(!parent.audio().loadMusic("troll","resources/troll.ogg"))
		return false;
	if(!Arrow::model.loadVoxelization("resources/arrow.vox"))
		return false;
	if(!Polla::model.loadVoxelization("resources/pene.vox"))
		return false;
	if(!Skeleton::model.loadVoxelization("resources/mob.vox"))
		return false;
	outLog("* Loading new world" );
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
	sf::Mouse::setPosition(sf::Vector2i(SCRWIDTH/2,SCRHEIGHT/2),parent.getWindow());
	// Add player
	addObject(player);
	outLog("* Init was succesful" );
	//setup shaders
	glActiveTexture(GL_TEXTURE0);
	parent.shaders().sendUniform1i("PROGRAM","tex0",0);
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
	world.update(deltaTime);
	for(std::list<GameObject*>::iterator it = objects.begin();it != objects.end(); ++it) {
		(*it)->update(deltaTime);
	}
	//Kill game objects far away from player
	for(std::list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
		if (std::abs(((*it)->pos-player->pos).module()) > 200)
			(*it)->isAlive = false;
	//Erase dead game objects
	for(std::list<GameObject*>::iterator it = objects.begin(); it != objects.end();)
		if (!(*it)->isAlive) {
			std::list<GameObject*>::iterator it2 = it;
			++it2;
			delete *it;
			objects.erase(it);
			it = it2;
		}
		else
			++it;
}

void SceneMain::draw() const {
	parent.shaders().useProgram("PROGRAM");
	//Move matrix to position (according to player)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(player->camRot.x, 1, 0, 0);
	glRotatef(player->camRot.y, 0, 1, 0);
	glTranslatef(-player->camPos.x, -player->camPos.y, -player->camPos.z);

	//Draw all the stuff
	parent.textures().useTexture("lolwtf");
	world.draw();
	for(std::list<GameObject*>::const_iterator it = objects.begin();it != objects.end(); ++it)
		(*it)->draw();

	glUseProgram(0);
	//Draw crosshair
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glColor3f(1,1,1);
	glVertex3f(-0.0002,      0,-0.01);
	glVertex3f( 0.0002,      0,-0.01);
	glVertex3f(      0,-0.0002,-0.01);
	glVertex3f(      0, 0.0002,-0.01);
	glEnd();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Debug tags
	parent.font().getText("posX").setString("X: " + toString(player->pos.x));
	parent.font().getText("posY").setString("Y: " + toString(player->pos.y));
	parent.font().getText("posZ").setString("Z: " + toString(player->pos.z));
	parent.font().getText("rotY").setString("Rot Y: " + toString(player->camRot.y));
	parent.font().getText("rotX").setString("Rot X: " + toString(player->camRot.x));
	parent.font().getText("Chunks").setString("Chunks drawn: " + toString(chunksDrawn));

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

void SceneMain::onKeyPressed(float deltaTime, sf::Keyboard::Key key) {
	switch(key) {
		case sf::Keyboard::E: { //new spawnpoint
			vec3f newPos(0,0,0);
			newPos.x = rand()%(WORLDWIDTH*CHUNKSIZE);
			newPos.z = rand()%(WORLDWIDTH*CHUNKSIZE);
			newPos.y = world.getSkylightLevel(newPos.x,newPos.z) + player->hitbox.radius.y + 0.5; //0.5 extra
			player->pos = newPos + vec3f(0.5,1,0.5);
		}
			break;
		case sf::Keyboard::Num1:
			player->selectedID = 1;
			break;
		case sf::Keyboard::Num2:
			player->selectedID = 2;
			break;
		case sf::Keyboard::Num3:
			player->selectedID = 3;
			break;
		case sf::Keyboard::Num4:
			player->selectedID = 4;
			break;
		case sf::Keyboard::Num5:
			player->selectedID = 5;
			break;
		case sf::Keyboard::Num6:
			player->selectedID = 6;
			break;
		case sf::Keyboard::Num7:
			player->selectedID = 7;
			break;
		case sf::Keyboard::Num8:
			player->selectedID = 8;
			break;
		case sf::Keyboard::Escape:
			parent.close();
			break;
		default:
			break;
	}
}

void SceneMain::onKeyDown(float deltaTime, sf::Keyboard::Key key) {
	//Move player
	const float vel = 10.0f;
	vec2f dir(sin(player->camRot.y*DEG_TO_RAD), -cos(player->camRot.y*DEG_TO_RAD));
	switch(key) {
		case sf::Keyboard::W:
			player->vel.x += dir.x*vel;
			player->vel.z += dir.y*vel;
			break;
		case sf::Keyboard::S:
			player->vel.x += -dir.x*vel;
			player->vel.z += -dir.y*vel;
			break;
		case sf::Keyboard::A:
			player->vel.x += dir.y*vel;
			player->vel.z += -dir.x*vel;
			break;
		case sf::Keyboard::D:
			player->vel.x += -dir.y*vel;
			player->vel.z += dir.x*vel;
			break;
		case sf::Keyboard::Space:
			if (player->onFloor && !player->isJumping)
				player->vel.y = 15;
			break;
		default:
			break;
	}
}

void SceneMain::onKeyReleased(float deltaTime, sf::Keyboard::Key key) {
	switch(key) {
		default:
			break;
	}
}

void SceneMain::onMouseButtonPressed(float deltaTime, sf::Mouse::Button button) {
	switch(button) {
		case sf::Mouse::Left: //delete block
			if(world.playerTargetsBlock) {
				world.setCubeID(world.targetedBlock.x,world.targetedBlock.y,world.targetedBlock.z,0);
			}
			break;
		case sf::Mouse::Right: //place block
			if(world.playerTargetsBlock) {
				world.setCubeID(world.last.x,world.last.y,world.last.z,player->selectedID);
			}
			break;
		case sf::Mouse::Middle: { //Arrow!
			float m[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, m);
			vec3f dir(m[2],m[6],m[10]);//same as the player's pov
			Polla * np = new Polla(this,player->camPos,player);
			np->vel -= vec3f(dir.x*30.0,dir.y*30.0,dir.z*30.0);
			addObject(np);

			break;
		}
		default:
			break;
	}
}

void SceneMain::onMouseButtonDown(float deltaTime, sf::Mouse::Button button) {
	switch(button) {
		default:
			break;
	}
}

void SceneMain::onMouseButtonReleased(float deltaTime, sf::Mouse::Button button) {
	switch(button) {
		default:
			break;
	}
}

void SceneMain::onMouseMoved(float deltaTime, int dx, int dy) {
	if (parent.input().focus){
		player->rotateX(dy*0.5);
		player->rotateY(dx*0.5);
		sf::Mouse::setPosition(sf::Vector2i(SCRWIDTH/2, SCRHEIGHT/2),parent.getWindow());
		parent.input().setMousePos(SCRWIDTH/2, SCRHEIGHT/2);
	}
}

void SceneMain::onClose() {
	outLog("* Closing scene: Main" );

	parent.textures().deleteTexture("lolwtf");
	parent.audio().deleteMusic("troll");
}

void SceneMain::addObject(GameObject* object) {
	objects.push_back(object);
}

World& SceneMain::getWorld() {
	return world;
}
