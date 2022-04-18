#include "OcultabarraButton.h"
#include "../../Data/ListaComandas.h"

OcultabarraButton::OcultabarraButton(UIManager* uim, Game* game, string texturename, int x, int y, int w, int h, ListaComandas* l) :UiButton(game, texturename, x, y, w, h)
{
	mlista = l;
}
OcultabarraButton::~OcultabarraButton()
{
}
void OcultabarraButton::execute(bool& exit)
{
	mlista->toggleBarra();
}