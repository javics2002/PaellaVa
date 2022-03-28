#include "Menu.h"

#include "../sdlutils/SDLUtils.h"
#include "../Control/Game.h"
#include "HostClient.h"

#include <iostream>

Menu::Menu(Game* game) : Scene(game)
{
	fondo->setTexture("menufondo");
	fondo->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	fondo->setDimension(sdlutils().width(), sdlutils().height()+100);

	int posIni = 260;
	int aumento = 150;

	auto startButton = new UiButton(game, "start", 640, posIni, 170, 100);
	startButton->setAction([](Game* game, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, game->UI);

		//Start game
		game->changeScene(new HostClient(game));
		});
	uiManager->addInterfaz(startButton);

	auto settingsButton = new UiButton(game, "settings", 640, posIni + aumento, 170, 100);
	settingsButton->setAction([](Game* game, bool& exit) {
		sdlutils().soundEffects().at("select").play(0, game->UI);
		});
	uiManager->addInterfaz(settingsButton);

	auto exitButton = new UiButton(game, "exit", 640, posIni + aumento * 2, 170, 100);
	exitButton->setAction([](Game* game, bool& exit) {
		sdlutils().soundEffects().at("cancel").play(0, game->UI);

		//exit game
		exit = true;
		});
	uiManager->addInterfaz(exitButton);
}

void Menu::handleInput(bool& exit)
{
	Scene::handleInput(exit);

	if (ih().getKey(InputHandler::CANCEL))
		exit = true;
}
