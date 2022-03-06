#include "Ingrediente.h"
#include "../Control/Game.h"

#include <iostream>

Ingrediente::Ingrediente(Game* game) :  PoolObject(game) {
	setDimension(DIMENSION, DIMENSION);
	setPosition(sdlutils().width() / 2, sdlutils().height());
}

void Ingrediente::update()
{
	setPosition(getX(), getY() - 1.3f);
}

//Qué hace este método?
std::list<PoolObject*>::const_iterator Ingrediente::ingredientCollide()
{
	deactivate();

	return getIterator();
}

void Ingrediente::onActivate()
{
	int n = rand() % (texturaIngrediente.size());

	setTexture(texturaIngrediente[n]);
}