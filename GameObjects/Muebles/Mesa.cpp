#include "Mesa.h"

Mesa::Mesa(Game* game, Vector2D<double> pos, Vector2D<int> dim, string texture) : Mueble(game, pos, dim.getX() * TILE_SIZE, dim.getY() * TILE_SIZE, texture) 
{
}
