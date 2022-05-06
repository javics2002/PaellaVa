#include "Arroz.h"
#include "../Control/Game.h"

Arroz::Arroz(Game* mGame) : ObjetoPortable(mGame)
{
    setDimension(DIMENSION, DIMENSION);
    setTexture("arroz");

    setDepth(2);
}
