#include "EnviaComandaButton.h"
#include "../../Control/Game.h"
#include "../../Data/Comanda.h"
#include "UIManager.h"
EnviaComandaButton::EnviaComandaButton(Game* game,string texturename, int x, int y, int w, int h) :UiButton(game, texturename, x, y, w, h)
{
	gamet = game;
	
}
EnviaComandaButton::~EnviaComandaButton()
{
}
void EnviaComandaButton::execute()
{
	std::cout << "comanda enviada";
	gamet->getUIManager()->getComanda()->enviaComanda();
}