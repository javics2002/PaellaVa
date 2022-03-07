#include "Silla.h"

Silla::Silla(Game* game, Vector2D<double> pos, string texture) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, texture)
{
}
