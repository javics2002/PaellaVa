#include "Disconnected.h"
#include "../Control/NetworkManager.h"
#include "../Scenes/Menu.h"

Disconnected::Disconnected(Game* game) : Scene(game)
{
	fondo->setTexture("disconnectedBg");
	fondo->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	fondo->setDimension(sdlutils().width(), sdlutils().height() + 100);

	UiButton* regresar = new UiButton(game, "back", 200, sdlutils().height() - 100, 280, 120);
	regresar->setInitialDimension(regresar->getWidth(), regresar->getHeight());
	regresar->setAction([](Game* game, bool& exit) {
		game->sendMessageScene(new Menu(game));
		});

	uiManager->addButton(regresar);

}



