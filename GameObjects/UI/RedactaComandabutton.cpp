#include "RedactaComandabutton.h"
#include "../../Control/game.h"
#include "../../Control/ObjectManager.h";

RedactaComandabutton::RedactaComandabutton(Game* game, string claveTextura, int x, int y, int w, int h) : UiButton(game, claveTextura, x, y, w, h)
{
	gamet = game;
}

RedactaComandabutton::~RedactaComandabutton()
{
}

void RedactaComandabutton::execute()
{
	//game->getObjectManager()->creaComanda(game);
	
}
