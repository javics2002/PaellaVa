#include "Numerobutton.h"
#include "../../Data/Comanda.h"
#include "UIManager.h"

NumeroButton::NumeroButton(UIManager* uim, Game* mGame, string texturename, int x, int y, int w, int h, int id) :UiButton(mGame, texturename, x, y, w, h)
{
	c = uim->getComanda();
	textura = texturename;
	idonline = id;
}

NumeroButton::~NumeroButton()
{
}

void NumeroButton::execute(bool& exit)
{
	c->anadirNumeromesa(textura, idonline);
	//Sonido de escribir aleatorio
	sdlutils().soundEffects().at("escribir" + std::to_string(sdlutils().rand().nextInt(2, 5))).play(0, mGame->UI);
}

void NumeroButton::update()
{
}