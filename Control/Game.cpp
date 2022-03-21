#include "Game.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "../Scenes/Menu.h"
#ifdef _DEBUG
#include "../Scenes/Restaurante.h"
#include "../Scenes/GameOver.h"
#endif // _DEBUG
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../Control/ObjectManager.h"
#include "../GameObjects/UI/UIManager.h"
#include "../Control/NetworkManager.h"

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
	getObjectManager()->initMuebles();
#else
	currentScene = new Menu(this);
#endif // _DEBUG

	// NETWORK
	bool decision = false;
	char nType = ' ';

	std::cout << "Host or client? (h/c)" << std::endl;

	while (!decision) {
		cin >> nType;
		if (nType == 'h' || nType == 'c')
			decision = true;
	}

	nm = new NetworkManager(this);

	if (nType == 'h') {
		nm->Init(nType);
	}
	else nm->Init(nType, "localhost");
	
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

	// update nm
	nm->Update();
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