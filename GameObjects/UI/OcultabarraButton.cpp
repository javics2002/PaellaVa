#include "OcultabarraButton.h"
#include "../../Data/ListaComandas.h"

OcultabarraButton::OcultabarraButton(UIManager* uim, Game* mGame, string texturename, int x, int y, int w, int h, ListaComandas* l) :UiButton(mGame, texturename, x, y, w, h)
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