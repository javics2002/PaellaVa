#include "Ingredientebutton.h"

Ingredientebutton::Ingredientebutton(Comanda* comanda,Game* game,string texturename, int x, int y, int w, int h):UiButton(game,texturename,x,y,w,h)
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
	comandat->aņadiraPedido(textuname);
	//HAY QUE CORTAR EL BUCLE DE INTERAZ AQUI POR QEU SI NO SE AŅADEN 6 INGREDIENTES AL PULDSAR UN BOTON XD
}
