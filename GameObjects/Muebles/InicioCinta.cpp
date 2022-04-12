#include "InicioCinta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

InicioCinta::InicioCinta(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, TILE_SIZE, "inicioCinta")
{
	initTime = sdlutils().virtualTimer().currTime();
}

void InicioCinta::update()
{
	int i= rand() % 1000;
	if (sdlutils().virtualTimer().currTime() - initTime >= SPAWN_DELAY)
	{
		if (true)
		{ 

			Ingredienteletal* i= game->getObjectManager()->getPool<Ingredienteletal>(_p_INGREDIENTELETAL)->add(getPosition());
			i->setVel(vel);
			initTime = sdlutils().virtualTimer().currTime();
		
		}
		else {
		/*	Ingrediente* i = game->getObjectManager()->getPool<Ingrediente>(_p_INGREDIENTE)->add(getPosition());
			i->setVel(vel);
			initTime = sdlutils().virtualTimer().currTime();*/
		}
	}
}

SDL_Rect InicioCinta::getCollider()
{
	return getTexBox();
}

SDL_Rect InicioCinta::getOverlap()
{
	return SDL_Rect();
}
