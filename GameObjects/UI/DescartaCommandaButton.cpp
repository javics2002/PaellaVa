#include "DescartaCommandaButton.h"
#include "../../Control/Game.h"
#include "../../Data/Comanda.h"

DescartaCommandaButton::DescartaCommandaButton(Comanda* c,Game* mGame, string texturename, int x, int y, int w, int h) : UiButton(mGame, texturename, x, y, w, h)
{
	comanda = c;
}

DescartaCommandaButton::~DescartaCommandaButton()
{
}

void DescartaCommandaButton::execute(bool& exit)
{
	
	comanda->cancelaPedido();

	sdlutils().soundEffects().at("comandaDescartada").play(0, mGame->UI);
}
