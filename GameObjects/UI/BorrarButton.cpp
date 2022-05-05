#include "BorrarButton.h"
#include "../../Control/Game.h"
#include "../../Data/Comanda.h"
BorrarButton::BorrarButton(Game* mGame, Comanda* c,string texturename, int x, int y, int w, int h) :UiButton(mGame, texturename, x, y, w, h)
{
	comanda = c;
}
BorrarButton::~BorrarButton()
{
}
void BorrarButton::execute(bool& exit)
{
	comanda->borraPedido();
	sdlutils().soundEffects().at("escribir5").play(0, mGame->UI);
}