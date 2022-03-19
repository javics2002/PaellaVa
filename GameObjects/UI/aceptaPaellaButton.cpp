#include "aceptaPaellaButton.h"
#include "../../Data/Comanda.h"

AceptaPaellaButton::AceptaPaellaButton(Game* game, Comanda* c, string texturename, int x, int y, int w, int h) :UiButton(game, texturename, x, y, w, h)
{
	comanda = c;
}

AceptaPaellaButton::~AceptaPaellaButton()
{

}
void AceptaPaellaButton::execute()
{
	std::cout << "paella aceptada";
	comanda->aceptaPaella();
	
}