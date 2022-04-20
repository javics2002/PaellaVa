#include "Game.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


#include "../Scenes/Menu.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../Control/ObjectManager.h"
#include "../GameObjects/UI/UIManager.h"
#include "../Control/NetworkManager.h"
#include "../Control/TextManager.h"


Game::Game() {	
	srand(time(NULL));
}

Game::~Game() {

}

void Game::init()
{
	SDLUtils::init("Paella va!", 1280, 720, "Assets/resources.json");
	SDLUtils::instance()->showCursor();

	ih().setGame(this);
	nm = new NetworkManager(this);

	currentScene = new Menu(this);
}

void Game::start()
{
	bool exit = false;
	SDL_Event event;

#ifndef _DEBUG
	//sdlutils().toggleFullScreen();
#endif // _DEBUG


	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		handleInput(event, exit);

		update();
		refresh();
		render();

		// Se procesan los mensajes en la cola
		recvMessageScene();
		
		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}

	nm->close();
}

void Game::handleInput(SDL_Event& event, bool& exit) {
	ih().update(event, exit);
	currentScene->handleInput(exit);
}

void Game::update()
{
	currentScene->update();

	if(nm != nullptr)
		nm->update();
}

void Game::refresh()
{
	currentScene->refresh();
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

NetworkManager* Game::getNetworkManager() {
	return nm;
}

void Game::sendMessageScene(Scene* newScene)
{
	MessageChangeScene msg;
	msg.newScene = newScene;

	qScene.push(msg);
}

void Game::recvMessageScene()
{
	while (!qScene.empty()) {
		delete currentScene;
		currentScene = qScene.front().newScene;
		qScene.pop();
	}
}

void Game::setNombre(string nombre_) {
	nombre = nombre_;
}

string Game::getNombre()
{
	return nombre;
}

void Game::setSlidesPos(Vector2D<double> slideVol_, Vector2D<double> slideSon_)
{
	slideSon = slideVol_;
	slideMus = slideSon_;
}

Vector2D<double> Game::getSlideSon()
{
	return slideSon;
}

Vector2D<double> Game::getSlideMus()
{
	return slideMus;
}
