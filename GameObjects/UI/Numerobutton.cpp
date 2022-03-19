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
void Numerobutton::execute()
{
	c->anadirNumeromesa(textura);
}
void Numerobutton::update()
{
}