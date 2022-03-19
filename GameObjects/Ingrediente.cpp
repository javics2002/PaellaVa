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
}

void Ingrediente::setProcesado(bool estadoIngr, Ingrediente ingr)
{
	procesado = estadoIngr;

	setTexture(texturaIngrProcesado[ingr.miTipo]);
}


tipoIngrediente Ingrediente::getTipo()
{
	return miTipo;
}

void Ingrediente::onObjectPicked()
{
	vel = { 0,0 };
}

void Ingrediente::onObjectDropped()
{
}



