#include "TorreCajaTakeaway.h"

#include "../CajaTakeaway.h"
#include "../Player.h"

#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

TorreCajaTakeaway::TorreCajaTakeaway(Game* mGame, Vector2D<double> pos) : Mueble(mGame, pos, TILE_SIZE, 2 * TILE_SIZE, "torreCajaTakeaway")
{
	//Siempre tiene que funcionar
	funcionando = true;
}

bool TorreCajaTakeaway::receiveCajaTakeaway(CajaTakeaway* c)
{
	c->setActive(false);
	return true;
}

bool TorreCajaTakeaway::returnObject(Player* p)
{
	p->setPickedObject(mGame->getObjectManager()->getPool<CajaTakeaway>(_p_CAJATAKEAWAY)->add(), CAJATAKEAWAY);

	return true;
}

