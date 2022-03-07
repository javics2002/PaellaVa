#include "Ingrediente.h"
#include "../Control/Game.h"

#include <iostream>

Ingrediente::Ingrediente(Game* game) :  PoolObject(game) {
	setDimension(DIMENSION, DIMENSION);
	setPosition(sdlutils().width() / 2, sdlutils().height());

	vel = { 0, 1.3 };
}

void Ingrediente::update()
{
	setPosition(getX(), getY() - vel.getY());
}

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

void Ingrediente::ingredienteRecogido()
{
	vel = { 0,0 };

	//cout << "vel " << vel.getX() << ", " << vel.getY() << endl;
}
