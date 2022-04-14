#include "InicioCinta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

#include "../../Control/NetworkManager.h"

InicioCinta::InicioCinta(Game* game, Vector2D<double> pos, bool host_) : Mueble(game, pos, TILE_SIZE, TILE_SIZE, "inicioCinta")
{
	initTime = sdlutils().virtualTimer().currTime();
	host = host_;
}

void InicioCinta::update()
{
	if (!host)
		return;


	int i = rand() % 1000;

	if (sdlutils().virtualTimer().currTime() - initTime >= SPAWN_DELAY && isActive())
	{
		if (i < porcentajeletal)
		{ 

			Ingredienteletal* i= game->getObjectManager()->getPool<Ingredienteletal>(_p_INGREDIENTELETAL)->add(getPosition());
			i->setVel(vel);
			initTime = sdlutils().virtualTimer().currTime();
		
		}
		else {
			Ingrediente* i = game->getObjectManager()->getPool<Ingrediente>(_p_INGREDIENTE)->add(getPosition());
			i->setVel(vel);
			initTime = sdlutils().virtualTimer().currTime();

			game->getNetworkManager()->sendCreateIngrediente(i->getTipo(), getPosition(), vel);
		}
	}
}

SDL_Rect InicioCinta::getCollider()
{
	if(isActive())return getTexBox();
	return { 0,0,0,0 };
}

SDL_Rect InicioCinta::getOverlap()
{
	return SDL_Rect();
}
