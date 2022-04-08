#include "Cinta.h"

Cinta::Cinta(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 9 * TILE_SIZE, "cinta")
{
	//Este sonido es m�s bajo
	sdlutils().soundEffects().at("cinta").setVolume(32);
	canalSonido = sdlutils().soundEffects().at("cinta").play(-1);
}

Cinta::~Cinta()
{
	sdlutils().soundEffects().at("cinta").haltChannel(canalSonido);
}

SDL_Rect Cinta::getCollider()
{
	return getTexBox();
}

SDL_Rect Cinta::getOverlap()
{
	return SDL_Rect();
}
