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

	currentScene = new Restaurante(this);
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

void Game::changeScene(Scene* scene) {
	//Cambio de escena 
	//currentScene = n;
}

void Game::handleInput(SDL_Event& event, bool& exit) {
	ih().clearState();
	while (SDL_PollEvent(&event))
		ih().update(event);

	//Salimos del juego (PROVISIONAL)
	exit = ih().GetKey(SDL_SCANCODE_ESCAPE);

	currentScene->handleInput();
}

void Game::update()
{
	currentScene->update();
}

void Game::render()
{
	sdlutils().clearRenderer();

	currentScene->render();
#ifdef _DEBUG
	currentScene->debug();
#endif // _DEBUG


	sdlutils().presentRenderer();
}

ObjectManager* Game::getObjectManager()
{
	return currentScene->getObjectManager();
}
UIManager* Game::getUIManager()
{
	return currentScene->getUIManager();
}