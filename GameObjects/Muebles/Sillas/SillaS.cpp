#include "SillaS.h"

#include"../../Cliente.h"

SillaS::SillaS(Game* game, Vector2D<double> pos, string texture)
	: Silla(game, pos, texture)
{
}

void SillaS::sentarCliente(Cliente* c)
{
	c->setPosition(getPosition().getX(), getPosition().getY() - getHeight() / 4 - 2);
	c->setPosVertical(20);
	c->setAnim(4);
}
