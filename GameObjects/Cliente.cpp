#include "Cliente.h"
#include "../../Control/game.h"

Cliente::Cliente(Game* game) : PoolObject(game)
{
	setDimension(DIMENSION, DIMENSION);
	vel.setX(2.0f);
	setColliderRect({ (int)getX(), (int)getY(), w/2, h/2});
}

void Cliente::update()
{
	setPosition(getX() + vel.getX(), getY());
	setColliderRect({ (int)getX(), (int)getY(), w, h });

	setColliderRect({ (int)getX(), (int)getY() + h/8, w / 2, h / 2 });
}

void Cliente::cambiaTextura(string textureN)
{
	setTexture(textureN);
}

void Cliente::clienteRecogido()
{
	vel = { 0,0 };
}

