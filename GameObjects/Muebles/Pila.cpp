#include "Pila.h"
#include "../Player.h"

Pila::Pila(Game* game, Vector2D<double> pos, TipoPaella miTipo_):Mueble(game, pos, 1 * TILE_SIZE, 2 * TILE_SIZE, "pila")
{
	miTipo = miTipo_;
}

bool Pila::returnObject(Player* p)
{
	if (paellas.front() != nullptr)
	{
		//TOCHECK: Podríamos hacer un return del objeto y que el player se lo guarde a sí mismo
		p->setPickedObject(paellas.front(), PAELLA);

		paellas.pop_front();

		return true;
	}
	
	return false;
}

bool Pila::receivePaella(Paella* pa)
{
	//if (miTipo == pa->getTipo() && pa->getContenido() == Limpia) {

	//	paellas.push_back(pa);

	//	return true;
	//}

	return false;
}
