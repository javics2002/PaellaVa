#include "BolsaArroz.h"

#include "../Arroz.h"
#include "../Player.h"

#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"


BolsaArroz::BolsaArroz(Game* mGame, Vector2D<double> pos) : Mueble(mGame, pos, TILE_SIZE, 2 * TILE_SIZE, "bolsaArroz")
{
	//Siempre tiene que funcionar
	funcionando = true;
}

bool BolsaArroz::returnObject(Player* p)
{
	if (p->getPickedObject() != nullptr)
		return false;
		
	p->setPickedObject(mGame->getObjectManager()->getPool<Arroz>(_p_ARROZ)->add(), ARROZ);
	return true;
}

bool BolsaArroz::receiveArroz(Arroz* arr)
{
	arr->setActive(false);
	return true;
}


