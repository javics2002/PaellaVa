#include "BorrarButton.h"
#include "../../Control/Game.h"
#include "../../Data/Comanda.h"
BorrarButton::BorrarButton(Game* game, string texturename, int x, int y, int w, int h) :UiButton(game, texturename, x, y, w, h)
{
	gamet = game;
}
BorrarButton::~BorrarButton()
{
}
void BorrarButton::execute()
{
	
}