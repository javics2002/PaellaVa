#include "CajaHerramientas.h"

#include "../Herramienta.h"
#include "../Player.h"

#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

CajaHerramientas::CajaHerramientas(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "extintor")
{
	//Siempre tiene que funcionar
	funcionando = true;
}

bool CajaHerramientas::receiveHerramienta(Herramienta* h)
{
	h->setActive(false);
	return true;
}

bool CajaHerramientas::returnObject(Player* p)
{
	p->setPickedObject(game->getObjectManager()->getPool<Herramienta>(_p_HERRAMIENTA)->add(), HERRAMIENTA);

	return true;
}

