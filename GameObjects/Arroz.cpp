#include "Arroz.h"
#include "../Control/Game.h"

Arroz::Arroz(Game* game) : ObjetoPortable(game), PoolObject()
{
    setDimension(DIMENSION, DIMENSION);
    setTexture("arroz");
}
