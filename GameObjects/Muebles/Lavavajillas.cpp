#include "Lavavajillas.h"

Lavavajillas::Lavavajillas(Game* game, Vector2D<double> pos) : Mueble(game, pos, 2 * TILE_SIZE, TILE_SIZE, "lavavajillas")
{

}

void Lavavajillas::onCollision()
{
	if (paellas < MAX_PAELLERAS)
		paellas++;
}

bool Lavavajillas::LavavajillasLleno()
{
	return paellas==MAX_PAELLERAS;
}

void Lavavajillas::metePaellera()
{
}

void Lavavajillas::sacaPaellera()
{
}

