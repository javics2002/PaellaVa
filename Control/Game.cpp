#include "Game.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "../Scenes/Restaurante.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../Control/ObjectManager.h"
#include "../GameObjects/UI/UIManager.h"

Game::Game() {
	srand(time(NULL));
}

Game::~Game() {
	
}

void Game::init() 
{
	SDLUtils::init("Paellas", 1280, 720, "../../../Assets/resources.json");

	SDLUtils::instance()->showCursor();

	restaurante = new Restaurante(this);
}

void Game::start()
{
	bool exit = false;
	SDL_Event event;

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		handleInput(event, exit);

		update();

		render();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}
}

void Game::changeScene(escenasJuego n) {
	//Cambio de escena 
	currentScene = n;
}

void Game::handleInput(SDL_Event &event, bool &exit) {
	ih().clearState();
	while (SDL_PollEvent(&event))
		ih().update(event);

	//Salimos del juego (PROVISIONAL)
	exit = ih().GetKey(SDL_SCANCODE_ESCAPE);

	if (currentScene == MENU) {
		//menu->handleInput();
	}
	else if (currentScene == RESTAURANTE) {
		restaurante->handleInput();
	}
}

void Game::update()
{
	if (currentScene == MENU) {
		//menu->update();
	}
	else if (currentScene == RESTAURANTE) {
		restaurante->update();
	}
}

void Game::render()
{
	sdlutils().clearRenderer();

	if (currentScene == MENU) {
		//menu->render();
	}
	else if (currentScene == RESTAURANTE) {
		restaurante->render();
#ifdef _DEBUG
		restaurante->debug();
#endif // _DEBUG
	}

	sdlutils().presentRenderer();
}

ObjectManager* Game::getObjectManager()
{
	return restaurante->getObjectManager();
}
UIManager* Game::getUIManager()
{
	return restaurante->getUIManager();
}