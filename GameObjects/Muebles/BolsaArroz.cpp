#include "BolsaArroz.h"
#include "../Arroz.h"
#include "../Player.h"
#include "../Paella.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

BolsaArroz::BolsaArroz(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "bolsaArroz")
{
	setColliderRect({ (int)getX(), (int)getY() - h / 4, w, 2 * h / 3 });
}

bool BolsaArroz::returnObject(Player* p)
{
	p->setPickedObject(game->getObjectManager()->getPoolArroces()->add(), ARROZ);

	return true;
}
