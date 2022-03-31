#include "Arroz.h"
#include "../Control/Game.h"

Arroz::Arroz(Game* game) : PoolObject(game)
{
    setDimension(DIMENSION, DIMENSION);
    setTexture("arroz");
}

void Arroz::update()
{
    setColliderRect({ (int)getX(), (int)getY(), w / 2, h / 2 });
}
