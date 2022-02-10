#include "Ingrediente.h"
#include "../Control/Game.h"

Ingrediente::Ingrediente(Game* game) :  PoolObject(game) {
	setDimension(DIMENSION, DIMENSION);
	setPosition(game->getWindowWidth() / 2, game->getWindowHeight());

	textureName = berenjenaTexture;
}

void Ingrediente::update()
{
	setPosition(getX(), getY() - 1.3f);
}

std::list<PoolObject*>::const_iterator Ingrediente::ingredientCollide()
{
	descativate();

	return getIterator();
}