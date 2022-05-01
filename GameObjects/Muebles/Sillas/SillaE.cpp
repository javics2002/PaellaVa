#include "SillaE.h"

#include"../../Cliente.h"

SillaE::SillaE(Game* game, Vector2D<double> pos, string texture) 
	: Silla(game, pos, texture)
{
}

void SillaE::sentarCliente(Cliente* c)
{
	c->setPosition(getPosition().getX(), getPosition().getY() - getHeight() / 4 + 2);
	c->setPosVertical(20);
	c->setAnim(4);
	c->setFlip(SDL_FLIP_NONE);
}
