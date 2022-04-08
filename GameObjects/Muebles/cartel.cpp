#include "cartel.h"
#include "../../Control/game.h"
#include "../../Control/ObjectManager.h"

Cartel::Cartel(Game* game, Vector2D<double> position) : Mueble(game, position, TILE_SIZE, TILE_SIZE, "cartel")
{
}

void Cartel::update()
{
	for (auto i : game->getObjectManager()->getPoolGrupoClientes()->getCollisions(getCollider())) {
		dynamic_cast<GrupoClientes*>(i)->colisionClientes();
	}
}

SDL_Rect Cartel::getCollider()
{
	return getTexBox();
}
