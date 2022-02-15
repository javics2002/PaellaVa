#include "Cliente.h"

Cliente::Cliente(Game* game) : PoolObject(game)
{
	setDimension(DIMENSION, DIMENSION);
	textureName = berenjenaTexture;
}

void Cliente::update()
{
	setPosition(getX() + 2.0f, getY());
}
