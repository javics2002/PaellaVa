#include "Ingrediente.h"
#include "../Control/Game.h"

#include <iostream>

Ingrediente::Ingrediente(Game* game) :  PoolObject(game) {
	setDimension(DIMENSION, DIMENSION);
}

void Ingrediente::update()
{
	setPosition(getX(), getY() + vel.getY());
	setColliderRect({ (int)getX(), (int)getY(), w / 2, h / 2});
}

pair<bool, std::list<PoolObject*>::const_iterator> Ingrediente::colisionIngrediente()
{
	return { !isPicked(), getIterator() };
}

void Ingrediente::onActivate()
{
	int n = rand() % (tipoIngrediente::LAST - 1) + 1; // Se le resta uno a LAST porque el arroz no debe salir en la
													  // cinta (siendo arroz = 0).

	vel = { 0, 1.3 };
	
	miTipo = tipoIngrediente(n);

	setTexture(texturaIngrediente[n]);
	setColliderRect({ (int)getX(), (int)getY(), w / 2, h / 2 });
}

void Ingrediente::onDesactivate()
{
	cout << "ing desactivado" << endl;
}

void Ingrediente::ingredienteRecogido()
{
}

void Ingrediente::setProcesado(bool estadoIngr, Ingrediente *ingr)
{
	procesado = estadoIngr;

	setTexture(texturaIngrProcesado[ingr->miTipo]);
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



