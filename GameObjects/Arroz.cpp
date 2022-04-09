#include "Arroz.h"
#include "../Control/Game.h"

Arroz::Arroz(Game* game) : ObjetoPortable(game)
{
    setDimension(DIMENSION, DIMENSION);
    setTexture("arroz");
}
