#include "SillaN.h"

#include"../../Cliente.h"

SillaN::SillaN(Game* mGame, Vector2D<double> pos, string texture)
	: Silla(mGame, pos, texture)
{
}

void SillaN::sentarCliente(Cliente* c)
{
	c->setPosition(getPosition().getX(), getPosition().getY() - getHeight() / 4 + 2);
	c->setPosVertical(20);
	c->setAnim(3);
}
