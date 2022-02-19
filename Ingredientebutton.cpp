#include "Ingredientebutton.h"

Ingredientebutton::Ingredientebutton(Comanda* comanda,Game* game, TextureName texturename, int x, int y, int w, int h):UiButton(game,texturename,x,y,w,h)
{

	gamet = game;
	comandat = comanda;
	textuname = texturename;

}
Ingredientebutton::~Ingredientebutton()
{
}
void Ingredientebutton::execute()
{
	comandat->añadiraPedido(textuname);
}
