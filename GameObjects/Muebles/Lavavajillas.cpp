#include "Lavavajillas.h"

Lavavajillas::Lavavajillas(Game* game, Vector2D<double> pos) : Mueble(game, pos, 2 * TILE_SIZE, TILE_SIZE, "lavavajillas")
{

}

void Lavavajillas::onCollision()
{
	if (paelleras < MAX_PAELLERAS)
		paelleras++;
}

bool Lavavajillas::LavavajillasLleno()
{
	return paelleras==MAX_PAELLERAS;
}

void Lavavajillas::metePaellera()
{
}

void Lavavajillas::sacaPaellera()
{
}

