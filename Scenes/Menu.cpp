#include "Menu.h"

#include "../sdlutils/SDLUtils.h"
#include "../Control/Game.h"
#include <iostream>

using namespace std;

Menu::Menu(Game* game)
{
	this->game = game;

	objectManager = new ObjectManager(game);


}

Menu::~Menu() 
{
	delete  objectManager;
}

void Menu::handleInput()
{
	objectManager->handleInput();
}

void Menu::update()
{
	objectManager->update();
}

void Menu::render()
{
	objectManager->render();
}

ObjectManager* Menu::getObjectManager()
{
	return objectManager;
}