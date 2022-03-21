#include "Tamanobutton.h"
#include"UIManager.h"

Tamanobutton::Tamanobutton(UIManager* uim, Game* game, string texturename, int x, int y, int w, int h) :UiButton(game, texturename, x, y, w, h)
{
	c = uim->getComanda();
	textura = texturename;
}
Tamanobutton::~Tamanobutton()
{
}
void Tamanobutton::execute()
{
	c->añadiraPedido(textura);
	c->toggleTecaldotam(false);
	c->toggleTeclado(true);
	
	//Sonido de escribir aleatorio
	sdlutils().soundEffects().at("escribir" + std::to_string(sdlutils().rand().nextInt(2, 5))).play(0, game->UI);
}
void Tamanobutton::update()
{
}

