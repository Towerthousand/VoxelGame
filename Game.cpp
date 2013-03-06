#include "Game.hpp"

Game::Game() : currentScene(NULL), nextScene(NULL) {
    window.create(sf::VideoMode(SCRWIDTH,SCRHEIGHT,32), WINDOW_TITLE ,sf::Style::Default,CONTEXT_SETTINGS_OPENGL);
	window.setMouseCursorVisible(false);
	window.setKeyRepeatEnabled(false);
	window.setVerticalSyncEnabled(false);
	WINDOWFOCUS = true;
	glClearColor(180.0/255.0,205.0/255.0,205.0/255.0,1);
}

Game::~Game() {
	//GAME DESTRUCTOR is actually a really cool name for the ultimate boss of a geeky
	//game about metagame. Nais. GAME CONSTRUCTOR is not a good hero name, though.
}

// Init non-resource, general game frame stuff here. It calls loadResources()
bool Game::init() {
	outLog("* INIT GAME" );

	//Load game resources
	if (!loadResources()) 
		return false;
	isRunning = true;

	//GL stuff..
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	//glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_LINE_SMOOTH);
	glEnable(GL_CULL_FACE); //enable backface culling
	glCullFace(GL_BACK);

	//initialise game-wide logic and objects
	fpsTime = 0.0;
	fpsCount = 0;
	outLog("* INIT GAME SUCCESFUL" );
	return true;
}

// Load scene-independent resources here, return false if failed to load
bool Game::loadResources () {
	if (!fontManager.globalFont.loadFromFile("resources/helvetica.ttf"))
		return false;
	return true;
}

// CAN'T TOUCH THIS NANANANA NANA NANA (main game loop)
void Game::run() {
	sf::Clock clock;
	while (isRunning) {
		float deltaTime = clock.restart().asSeconds();
		update(deltaTime);
		draw();
	}
}

// 1: Change scene if necessary
// 2: Update game-wide logic
// 3: Process input
// 4: Update scene
void Game::update(float deltaTime) {
	
	//Change scene, initialise it and close if it fails to initialise
	if (nextScene != NULL) {
			if (currentScene != NULL) 
				delete currentScene;
			currentScene = nextScene;
			nextScene = NULL;
			if (!currentScene->init()) {
				delete currentScene;
				currentScene = NULL; 
				onClose();
			}
	}

	//FPS Count and title (game-wide logic)
	if (fpsTime > 1) {
		window.setTitle(std::string(WINDOW_TITLE) + ", fps: " + toString(fpsCount));
		fpsTime = 0;
		fpsCount = 0;
	}
	fpsTime += deltaTime;
	++fpsCount;
	
	//Scene logic updating
	if (currentScene != NULL) 
		currentScene->update(deltaTime);

	//Check window events
	sf::Event event;
	while(window.pollEvent(event)) {
		switch(event.type) {
			case sf::Event::Closed:
				onClose();
				break;
			case sf::Event::Resized:
				// adjust the viewport when the window is resized
				SCRWIDTH = event.size.width;
				SCRHEIGHT = event.size.height;
				glViewport(0, 0, SCRWIDTH, SCRHEIGHT);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluPerspective(60.0f, float(SCRWIDTH)/float(SCRHEIGHT), 0.01f, 500.0f);
				break;
			case sf::Event::GainedFocus:
				WINDOWFOCUS = true;
				break;
			case sf::Event::LostFocus:
				WINDOWFOCUS = false;
				break;
			case sf::Event::MouseButtonPressed:
				onMouseButtonPressed(deltaTime, event);
				break;
			case sf::Event::KeyPressed:
				onKeyPressed(deltaTime, event);
				if (event.key.code == sf::Keyboard::Escape)
					onClose();
				break;
			case sf::Event::MouseMoved:
				onMouseMoved(deltaTime,event);
				break;
			default:
				break;
		}
	}
}

// Draw scene
void Game::draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (currentScene != NULL)
		currentScene->draw();
	window.display();
}

// Pass the time elapsed to scene so it can handle key imput
void Game::onKeyPressed(float deltaTime, sf::Event event) {
	if (currentScene != NULL) 
		currentScene->onKeyPressed(deltaTime, event);
}

// Pass the time elapsed to scene and mouse event so it can handle mouse imput
void Game::onMouseButtonPressed(float deltaTime, sf::Event event) {
	if (currentScene != NULL) 
		currentScene->onMouseButtonPressed(deltaTime, event);
}

void Game::onMouseMoved(float deltaTime, sf::Event event) {
	if (currentScene != NULL)
		currentScene->onMouseMoved(deltaTime,event);
}

// Whenever you wnat to ende the game, you must call this function, NOT close()
void Game::onClose() {
	if (currentScene != NULL)
		currentScene->onClose();
	outLog("* EXITING GAME" );
	close();
}

// End game-wide stuff here
void Game::close() {
	if (currentScene != NULL){
		delete currentScene;
		currentScene = NULL;
	}
	window.close();
	isRunning = false;
}

// Change scene so that on next Game::update(), currentScene will be replaced
void Game::setScene (Scene * scene) {
	if(nextScene != NULL)
		delete nextScene;
	nextScene = scene;
}
