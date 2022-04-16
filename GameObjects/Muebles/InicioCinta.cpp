#include "InicioCinta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"
#include "../../Scenes/Tutorial.h"
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
		if (i < porcentajeLetal && !dynamic_cast<Tutorial*>(game->getCurrentScene()))
		{ 
			IngredienteLetal* ingLetal = game->getObjectManager()->getPool<IngredienteLetal>(_p_INGREDIENTELETAL)->add(getPosition());
			ingLetal->setVel(vel);
			initTime = sdlutils().virtualTimer().currTime();

			game->getNetworkManager()->sendCreateIngredienteLetal(ingLetal->getTipo(), getPosition(), vel);
		}
		else {
			Ingrediente* ing = game->getObjectManager()->getPool<Ingrediente>(_p_INGREDIENTE)->add(getPosition());
			ing->setVel(vel);
			initTime = sdlutils().virtualTimer().currTime();

			game->getNetworkManager()->sendCreateIngrediente(ing->getTipo(), getPosition(), vel);
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
