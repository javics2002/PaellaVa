#include "Pared.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

Pared::Pared(Game* mGame, Vector2D<double> position) : Mueble(mGame, position, TILE_SIZE, TILE_SIZE, "cartel")
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

SDL_Rect Pared::getOverlap()
{
	return SDL_Rect();
}