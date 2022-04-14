#include "Ingrediente.h"

#include <iostream>

Ingrediente::Ingrediente(Game* game) : ObjetoPortable(game) {
	setDimension(DIMENSION, DIMENSION);

	setDepth(2);
}

void Ingrediente::update()
{
	setPosition(pos+vel);
}

void Ingrediente::onActivate()
{
	int n = rand() % (tipoIngrediente::LAST) ;

	miTipo = tipoIngrediente(n);

	setTexture(texturaIngrediente[n]);
}


void Ingrediente::onDeactivate()
{
}

void Ingrediente::cambiaTipo(int n) {

	miTipo = tipoIngrediente(n);

	setTexture(texturaIngrediente[n]);
}

void Ingrediente::ingredienteRecogido()
{
}

void Ingrediente::setProcesado(bool estadoIngr, Ingrediente *ingr)
{
	procesado = estadoIngr;

	setTexture(texturaIngrProcesado[ingr->miTipo]);
}


bool Ingrediente::getProcesado()
{
	return procesado;
}
bool Ingrediente::esLetal()
{
	return letal;
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



