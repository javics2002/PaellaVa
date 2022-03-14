#include "Game.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "../Scenes/Restaurante.h"
#include "../Scenes/Menu.h"
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

#ifdef _DEBUG
	currentScene = new Restaurante(this);
#else
	currentScene = new Menu(this);
#endif // _DEBUG
}

void Game::start()
{
	bool exit = false;
	SDL_Event event;

#ifndef _DEBUG
	sdlutils().toggleFullScreen();
#endif // _DEBUG


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
	currentScene = nullptr;
	currentScene = scene;
}

void Game::handleInput(SDL_Event& event, bool& exit) {
	while (SDL_PollEvent(&event))
		ih().update(event);

#ifdef _DEBUG
	exit = ih().getKey(SDL_SCANCODE_ESCAPE);
#endif // _DEBUG

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