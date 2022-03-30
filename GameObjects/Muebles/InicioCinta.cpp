#include "InicioCinta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

InicioCinta::InicioCinta(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, TILE_SIZE, "inicioCinta")
{
	time = SDL_GetTicks();
}

void InicioCinta::update()
{
	if (SDL_GetTicks() - time >= SPAWN_DELAY) 
	{
		game->getObjectManager()->getPoolIngredientes()->add(getPosition());
		time = SDL_GetTicks();
	}
}
