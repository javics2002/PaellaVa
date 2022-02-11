#include "Comanda.h"
Comanda::Comanda(Game* game,uint nmesa):GameObject(game)
{
	textureName = cuadernillo;
	Vector2D<double> p;
	p.setX(100);
	p.setY(100);
	setPosition(p);
	setDimension(100, 200);

}