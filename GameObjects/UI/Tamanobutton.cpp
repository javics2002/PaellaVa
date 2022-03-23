#include "Tamanobutton.h"
#include"UIManager.h"

TamanoButton::TamanoButton(UIManager* uim, Game* game, string texturename, int x, int y, int w, int h) :UiButton(game, texturename, x, y, w, h)
{
	c = uim->getComanda();
	textura = texturename;
}

TamanoButton::~TamanoButton()
{
}

void TamanoButton::execute(bool& exit)
{
	c->añadiraPedido(textura);
	c->toggleTecaldotam(false);
	c->toggleTeclado(true);
	
	//Sonido de escribir aleatorio
	sdlutils().soundEffects().at("escribir" + std::to_string(sdlutils().rand().nextInt(2, 5))).play(0, game->UI);
}

void TamanoButton::update()
{
}

