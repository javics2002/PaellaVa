#include "Game.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "../Scenes/Restaurante.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

Game::Game() {
	srand(time(NULL));
}

Game::~Game() {
	delete objectManager;
}

void Game::init() 
{
	SDLUtils::init("Paellas", 1080, 720, "../../../Assets/resources.json");

    objectManager = new ObjectManager(this);
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

void Game::handleInput(SDL_Event &event, bool &exit) {
	ih().clearState();
	while (SDL_PollEvent(&event))
		ih().update(event);

	//Salimos del juego (PROVISIONAL)
	exit = ih().isKeyDown(SDL_SCANCODE_ESCAPE);

	objectManager->handleInput();
}

void Game::update()
{
	objectManager->update();
}

void Game::render()
{
	sdlutils().clearRenderer();

	objectManager->render();

	sdlutils().presentRenderer();
}

ObjectManager* Game::getObjectManager()
{
	return objectManager;
}
