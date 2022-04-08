#include "Pared.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

Pared::Pared(Game* game, Vector2D<double> position) : Mueble(game, position, TILE_SIZE, TILE_SIZE, "cartel")
{
	setDimension(64, 64);
}

Pared::~Pared()
{
}


SDL_Rect Pared::getCollider()
{
	return getTexBox();
}
