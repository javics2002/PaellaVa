#include "Ingrediente.h"
#include "../Control/Game.h"

#include <iostream>

Ingrediente::Ingrediente(Game* game) :  PoolObject(game) {
	setDimension(DIMENSION, DIMENSION);
}

void Ingrediente::update()
{
	setPosition(getX(), getY() + vel.getY());
}

std::list<PoolObject*>::const_iterator Ingrediente::ingredientCollide()
{
	deactivate();

	return getIterator();
}

void Ingrediente::onActivate()
{
	int n = rand() % (texturaIngrediente.size());

	vel = { 0, 1.3 };
	
	setTexture(texturaIngrediente[n]);
}

void Ingrediente::ingredienteRecogido()
{
	vel = { 0,0 };
}

void Ingrediente::setProcesado(bool estadoIngr)
{
	procesado = estadoIngr;
}


