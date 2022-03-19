#include "Ingredientebutton.h"
#include "../UI/UIManager.h"
Ingredientebutton::Ingredientebutton(UIManager* uim, Game* game, string texturename, int x, int y, int w, int h) : UiButton(game, texturename, x, y, w, h)
{
	this->uim = uim;
}
Ingredientebutton::~Ingredientebutton()
{
}
void Ingredientebutton::execute()
{
	uim->getComanda()->añadiraPedido(textura);
	uim->randomizaTeclado();

	//comandat->añadiraPedido(textuname);
	//HAY QUE CORTAR EL BUCLE DE INTERAZ AQUI POR QEU SI NO SE AÑADEN 6 INGREDIENTES AL PULDSAR UN BOTON XD
	//well podria ser un while en ligar d eun for
}
void Ingredientebutton::update()
{
}
