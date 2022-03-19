#include "GameOver.h"
#include "../GameObjects/UI/Text.h"

GameOver::GameOver(Game* game) : Scene(game)
{
	fondo->setTexture("menufondo");
	fondo->setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	fondo->setDimension(sdlutils().width(), sdlutils().height() + 100);

	uiManager->addInterfaz(new Text(game, string("hola"), string("paella")));
}
