#include "BolsaArroz.h"
#include "../Arroz.h"
#include "../Player.h"
#include "../Paella.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

BolsaArroz::BolsaArroz(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "arroz")
{

}

bool BolsaArroz::returnObject(Player* p)
{
	Arroz* arroz = new Arroz(game, "alcachofa");
	game->getObjectManager()->addArroz(arroz);
	
	p->setPickedObject(arroz, ARROZ);


	return true;

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
