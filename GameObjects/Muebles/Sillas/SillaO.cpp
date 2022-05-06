#include "SillaO.h"

#include"../../Cliente.h"

SillaO::SillaO(Game* mGame, Vector2D<double> pos, string texture)
	: Silla(mGame, pos, texture)
{
}

void SillaO::sentarCliente(Cliente* c)
{
	c->setPosition(getPosition().getX(), getPosition().getY() - getHeight() / 4 + 2);
	c->setPosVertical(20);
	c->setAnim(4);
	c->setFlip(SDL_FLIP_HORIZONTAL);
}
