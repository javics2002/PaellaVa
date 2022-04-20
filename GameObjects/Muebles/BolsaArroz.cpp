#include "BolsaArroz.h"

#include "../Arroz.h"
#include "../Player.h"

#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"


BolsaArroz::BolsaArroz(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "bolsaArroz")
{
	//Siempre tiene que funcionar
	funcionando = true;
}

bool BolsaArroz::returnObject(Player* p)
{
	p->setPickedObject(game->getObjectManager()->getPool<Arroz>(_p_ARROZ)->add(), ARROZ);

	return true;
}
