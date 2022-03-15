#include "aceptaPaellaButton.h"
#include "../../Data/Comanda.h"

aceptaPaellaButton::aceptaPaellaButton(Game* game, Comanda* c, string texturename, int x, int y, int w, int h) :UiButton(game, texturename, x, y, w, h)
{
	gamet = game;
	comandat = c;
}

aceptaPaellaButton::~aceptaPaellaButton()
{

}
void aceptaPaellaButton::execute()
{
	std::cout << "paella aceptada";
	comandat->aceptaPaella();
	
}