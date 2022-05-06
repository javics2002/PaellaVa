#include "aceptaPaellaButton.h"
#include "../../Data/Comanda.h"

AceptaPaellaButton::AceptaPaellaButton(Game* mGame, Comanda* c, string texturename, int x, int y, int w, int h) :UiButton(mGame, texturename, x, y, w, h)
{
	comanda = c;
}

AceptaPaellaButton::~AceptaPaellaButton()
{

}

void AceptaPaellaButton::execute(bool& exit)
{
	comanda->aceptaPaella();
	
	sdlutils().soundEffects().at("escribir1").play(0, mGame->UI);
}