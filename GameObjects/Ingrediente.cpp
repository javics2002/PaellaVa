#include "Ingrediente.h"
#include "../Control/Game.h"

#include <iostream>

Ingrediente::Ingrediente(Game* game) :  PoolObject(game), tipoIngrediente(-1) {
	setDimension(DIMENSION, DIMENSION);
	setPosition(game->getWindowWidth() / 2, game->getWindowHeight());
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

void Ingrediente::onActivate()
{
	auto pair = game->getRandomIngridient();

	textureName = pair.first;
	tipoIngrediente = pair.second;
}