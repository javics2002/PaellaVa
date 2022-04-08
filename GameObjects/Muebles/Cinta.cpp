#include "Cinta.h"

Cinta::Cinta(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 9 * TILE_SIZE, "cinta")
{
	//Este sonido es más bajo
	//canalSonido = sdlutils().soundEffects().at("cinta").play(-1);
	sdlutils().soundEffects().at("cinta").setVolume(32);
}
