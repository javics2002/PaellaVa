#include "Cliente.h"
#include "../../Control/game.h"

Cliente::Cliente(Game* game) : PoolObject(game)
{
	setDimension(DIMENSION, DIMENSION);
	textureName = berenjenaTexture;
}

void Cliente::update()
{
	setPosition(getX() + 2.0f, getY());

}

void Cliente::cambiaTextura(TextureName textureN)
{
	textureName = textureN;
}
