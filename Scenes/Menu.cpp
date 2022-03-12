#include "Menu.h"

#include "../sdlutils/SDLUtils.h"
#include "../Control/Game.h"
#include "../GameObjects/UI/StartButton.h"
#include "../GameObjects/UI/OpcionesButton.h"
#include "../GameObjects/UI/ExitButton.h"
#include "../GameObjects/UI/CustomizationButton.h"

#include <iostream>

using namespace std;

Menu::Menu(Game* game) : Scene(game)
{
	this->game = game;

	objectManager = new ObjectManager(game);
	uiManager = new UIManager(game);



	// camara init
	camara = new Camera(*new Vector2D<float>(0, 16), sdlutils().width(), sdlutils().height());

	getUIManager()->addInterfaz(new StartButton(game, "start", 640, 360, 100, 100));
	getUIManager()->addInterfaz(new OpcionesButton(game, "settings", 640, 460, 100, 100));
	getUIManager()->addInterfaz(new ExitButton(game, "exit", 640, 560, 100, 100));
	getUIManager()->addInterfaz(new CustomizationButton(game, "customization", 640, 660, 100, 100));


}

Menu::~Menu()
{
	delete  objectManager;
	delete uiManager;
}

void Menu::render()
{
	objectManager->debug(camara->renderRect());
	uiManager->render(nullptr);
}

void Menu::debug()
{
	objectManager->debug(camara->renderRect());
}