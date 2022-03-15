#include "BorrarButton.h"
#include "../../Control/Game.h"
#include "../../Data/Comanda.h"
BorrarButton::BorrarButton(Game* game, Comanda* c,string texturename, int x, int y, int w, int h) :UiButton(game, texturename, x, y, w, h)
{
	gamet = game;
	comandat = c;
}
BorrarButton::~BorrarButton()
{
}
void BorrarButton::execute()
{
	comandat->borraPedido();
}