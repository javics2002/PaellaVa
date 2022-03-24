#include "BolsaArroz.h"
#include "../../Control/Game.h"

BolsaArroz::BolsaArroz(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "arroz")
{
	arroces = new Pool<Ingrediente>(game, 20);
}

bool BolsaArroz::returnObject(Player* p)
{
	//if (paellas.front() != nullptr)
	//{
	//	//TOCHECK: Podríamos hacer un return del objeto y que el player se lo guarde a sí mismo
	//	p->setPickedObject(paellas.front(), PAELLA);

	//	paellas.pop_front();

	//	return true;
	//}

	return false;
}

//bool BolsaArroz::receivePaella(Paella* pa)
//{
//	//if (miTipo == pa->getTipo() && pa->getContenido() == Limpia) {
//
//	//	paellas.push_back(pa);
//
//	//	return true;
//	//}
//
//	return false;
//}
