#include "InicioCinta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

InicioCinta::InicioCinta(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, TILE_SIZE, "inicioCinta")
{
	time = sdlutils().virtualTimer().currTime();
}

void InicioCinta::update()
{
	if (sdlutils().virtualTimer().currTime() - (time - offsetTime) >= SPAWN_DELAY)
	{
		game->getObjectManager()->getPoolIngredientes()->add(getPosition());
		time = sdlutils().virtualTimer().currTime();
		offsetTime = 0;
	}
}
