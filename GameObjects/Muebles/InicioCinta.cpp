#include "InicioCinta.h"
#include "../../Control/Game.h"

InicioCinta::InicioCinta(Game* game, Pool<Ingrediente>* pool) : GameObject(game), poolIngredientes(pool)
{
	setPosition(sdlutils().width() - DIMENSION_W / 2, sdlutils().height() - DIMENSION_H / 2);
	setDimension(DIMENSION_W, DIMENSION_H);

	time = SDL_GetTicks();

	setTexture("inicioCinta");
}

void InicioCinta::update()
{
	if (SDL_GetTicks() - time >= SPAWN_DELAY) {
		poolIngredientes->add(getPosition());
		time = SDL_GetTicks();
	}
}
