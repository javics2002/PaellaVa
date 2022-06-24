#include "CajaHerramientas.h"

#include "../Herramienta.h"
#include "../Player.h"

#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

CajaHerramientas::CajaHerramientas(Game* mGame, Vector2D<double> pos) : Mueble(mGame, pos, TILE_SIZE, 2 * TILE_SIZE, "cajaHerramientas")
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
	if (p->getPickedObject() != nullptr)
		return false;

	p->setPickedObject(mGame->getObjectManager()->getPool<Herramienta>(_p_HERRAMIENTA)->add(), HERRAMIENTA);
	return true;
}

