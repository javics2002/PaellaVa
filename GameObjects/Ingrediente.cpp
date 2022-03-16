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
	int n = rand() % (tipoIngrediente::LAST);

	vel = { 0, 1.3 };
	
	miTipo = tipoIngrediente(n);

	setTexture(texturaIngrediente[n]);
}

void Ingrediente::ingredienteRecogido()
{
	vel = { 0,0 };

	cogido = true;
}

void Ingrediente::setProcesado(bool estadoIngr)
{
	procesado = estadoIngr;
}

void Ingrediente::setCogido(bool cogido_)
{
	cogido = cogido_;
}

bool Ingrediente::getCogido()
{
	return cogido;
}

tipoIngrediente Ingrediente::getTipo()
{
	return miTipo;
}


