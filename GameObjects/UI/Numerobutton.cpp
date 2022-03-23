#include "Numerobutton.h"
#include "../../Data/Comanda.h"
#include "UIManager.h"

Numerobutton::Numerobutton(UIManager* uim, Game* game, string texturename, int x, int y, int w, int h) :UiButton(game, texturename, x, y, w, h)
{
	c = uim->getComanda();
	textura = texturename;
}

Numerobutton::~Numerobutton()
{
}

void Numerobutton::execute(bool& exit)
{
	c->anadirNumeromesa(textura);

	//Sonido de escribir aleatorio
	sdlutils().soundEffects().at("escribir" + std::to_string(sdlutils().rand().nextInt(2, 5))).play(0, game->UI);
}

void Numerobutton::update()
{
}