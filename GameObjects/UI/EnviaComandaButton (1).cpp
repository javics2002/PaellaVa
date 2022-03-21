#include "EnviaComandaButton.h"
#include "../../Control/Game.h"
#include "../../Data/Comanda.h"
#include "UIManager.h"
EnviaComandaButton::EnviaComandaButton(Game* game,string texturename, int x, int y, int w, int h) :UiButton(game, texturename, x, y, w, h)
{
}
EnviaComandaButton::~EnviaComandaButton()
{
}
void EnviaComandaButton::execute()
{
	std::cout << "comanda enviada";
	game->getUIManager()->getComanda()->enviaComanda();
	sdlutils().soundEffects().at("comandaEnviada").play(0, game->UI);
}