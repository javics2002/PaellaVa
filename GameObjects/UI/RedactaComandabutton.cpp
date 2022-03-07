#include "RedactaComandabutton.h"
#include "../../Control/game.h"
#include "../../Control/ObjectManager.h";
#include "../../GameObjects/UI/UIManager.h"

RedactaComandabutton::RedactaComandabutton(Game* game, string claveTextura, int x, int y, int w, int h) : UiButton(game, claveTextura, x, y, w, h)
{
	gamet = game;
	//uim = game->getUIManager();
}

RedactaComandabutton::~RedactaComandabutton()
{
}

void RedactaComandabutton::execute()
{
	uim = gamet->getUIManager();
	uim->creaComanda(gamet);

}

