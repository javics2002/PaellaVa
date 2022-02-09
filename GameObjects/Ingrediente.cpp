#include "Ingrediente.h"
#include "../Control/Game.h"

Ingrediente::Ingrediente(Game* game) : active(false), GameObject(game) {
	setDimension(DIMENSION, DIMENSION);
	setPosition(game->getWindowWidth() / 2, game->getWindowHeight());

	textureName = berenjenaTexture;
}


void Ingrediente::activate()
{
	active = true;
}

void Ingrediente::descativate()
{
	active = false;
}

bool Ingrediente::isActive()
{
	return active;
}

void Ingrediente::update()
{
	setPosition(getX(), getY() - 1);
}