#include "EnviaComandaButton.h"
#include "../../Control/Game.h"
#include "../../Data/Comanda.h"
#include "UIManager.h"
EnviaComandaButton::EnviaComandaButton(Game* mGame,string texturename, int x, int y, int w, int h) :UiButton(mGame, texturename, x, y, w, h)
{
}
EnviaComandaButton::~EnviaComandaButton()
{
}
void EnviaComandaButton::execute(bool& exit)
{
	//std::cout << "comanda enviada";
	mGame->getUIManager()->getComanda()->enviaComanda();
	sdlutils().soundEffects().at("comandaEnviada").play(0, mGame->UI);
}