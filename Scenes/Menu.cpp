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

	fondo = new Fondo(game);
	fondo->setTexture("menufondo");
	fondo->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	fondo->setDimension(sdlutils().width(), sdlutils().height()+100);


	// camara init
	camara = new Camera(*new Vector2D<float>(0, 16), sdlutils().width(), sdlutils().height());
	int posIni = 260;
	int aumento = 150;
	getUIManager()->addInterfaz(new StartButton(game, "start", 640, posIni, 170, 100));
	getUIManager()->addInterfaz(new OpcionesButton(game, "settings", 640, posIni+aumento, 170, 100));
	getUIManager()->addInterfaz(new ExitButton(game, "exit", 640, posIni+aumento*2, 170, 100));


}

Menu::~Menu()
{
	delete  objectManager;
	delete uiManager;
}

void Menu::render()
{
	fondo->render(camara->renderRect());

	objectManager->debug(camara->renderRect());
	uiManager->render(nullptr);
}

void Menu::debug()
{
	objectManager->debug(camara->renderRect());
}