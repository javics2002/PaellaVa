#include "InicioCinta.h"
#include "../../Control/Game.h"

InicioCinta::InicioCinta(Game* game, Pool<Ingrediente>* pool) : GameObject(game), poolIngredientes(pool)
{
	setPosition(game->getWindowWidth() - DIMENSION_W / 2, game->getWindowHeight() - DIMENSION_H / 2);
	setDimension(DIMENSION_W, DIMENSION_H);

	time = SDL_GetTicks();

	textureName = inicioCintaTexture;
}

void InicioCinta::update()
{
	if (SDL_GetTicks() - time >= SPAWN_DELAY) {
		poolIngredientes->add(getPosition());
		time = SDL_GetTicks();
	}
}
