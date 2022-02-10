#include "Game.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "../Scenes/Restaurante.h"
#include "../sdlutils/SDLUtils.h"

Game::Game( int width, int height) {
	name = "Paellas Rodriguez";
	this->width = width;
	this->height = height;

	doExit = false;
	//font = new Font("../Images/Monospace.ttf", 12);

	srand(time(NULL));

	startGame();
}

void Game::startGame() {
	SDL_Texture* spriteSheet = IMG_LoadTexture(renderer, "Assets/Tileset.png");
	Restaurante().Render() ;


	objectManager = new ObjectManager(this);
}

Game::~Game() {
	delete objectManager;
	delete textureContainer;
}

string Game::getGameName()
{
	return name;
}

void Game::update() 
{
	objectManager->update();
}

void Game::draw()
{
	objectManager->render();
	objectManager->debug();
}

void Game::setUserExit() {
	doExit = true;
}

bool Game::isUserExit() {
	return doExit;
}

int Game::getWindowWidth() {
	return width;
}

int Game::getWindowHeight() {
	return height;
}

SDL_Renderer* Game::getRenderer() {
	return renderer;
}

void Game::setRenderer(SDL_Renderer* _renderer) {
	renderer = _renderer;
}

void Game::loadTextures() {
	if (renderer == nullptr)
		throw string("Renderer is null");

	textureContainer = new TextureContainer(renderer);
}

//void Game::renderText(string text, int x, int y, SDL_Color color) {
//	font->render(renderer, text.c_str(), x, y, color);
//}

Texture* Game::getTexture(TextureName name) {
	return textureContainer->getTexture(name);
}

pair<TextureName, int> Game::getRandomIngridient()
{
	return objectManager->getRandomIngridient();
}
ObjectManager* Game::getObjectManager()
{
	return objectManager;
}
