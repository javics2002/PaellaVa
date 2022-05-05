#include "Tamanobutton.h"
#include"UIManager.h"

TamanoButton::TamanoButton(UIManager* uim, Game* mGame, string texturename, int x, int y, int w, int h,int id) :UiButton(mGame, texturename, x, y, w, h)
{
	c = uim->getComanda();
	textura = texturename;
	idonline = id;
}

TamanoButton::~TamanoButton()
{
}

void TamanoButton::execute(bool& exit)
{
	c->añadiraPedido(textura,idonline);
	c->toggleTecaldotam(false);
	c->toggleTeclado(true);
	c->setActiveTeclado(c->getTecladoing());
	//Sonido de escribir aleatorio
	sdlutils().soundEffects().at("escribir" + std::to_string(sdlutils().rand().nextInt(2, 5))).play(0, mGame->UI);
}

void TamanoButton::update()
{
}

