#include "Numerobutton.h"
#include "../../Data/Comanda.h"
#include "UIManager.h"

NumeroButton::NumeroButton(UIManager* uim, Game* game, string texturename, int x, int y, int w, int h, int id) :UiButton(game, texturename, x, y, w, h)
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
	sdlutils().soundEffects().at("escribir" + std::to_string(sdlutils().rand().nextInt(2, 5))).play(0, game->UI);
}

void NumeroButton::update()
{
}