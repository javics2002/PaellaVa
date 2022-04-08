#include "Arroz.h"
#include "../Control/Game.h"

Arroz::Arroz(Game* game) : PoolObject(game)
{
    setDimension(DIMENSION, DIMENSION);
    setTexture("arroz");
}
