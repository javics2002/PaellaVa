#include "IngredienteLetal.h"
IngredienteLetal::IngredienteLetal(Game* game) : Ingrediente(game)
{
	setDimension(DIMENSION, DIMENSION);
	setDepth(2);
	letal = true;
}
IngredienteLetal::~IngredienteLetal()
{
}

void IngredienteLetal::onActivate()
{
	int n = rand() % (tipoIngredienteLetal::LEST); 

	miTipoL = tipoIngredienteLetal(n);

	setTexture(texturaIngredienteLetal[n]);
}

void IngredienteLetal::setProcesado(bool estadoIngr, Ingrediente* ingr)
{
	procesado = estadoIngr;

	setTexture("rataP");
}

void IngredienteLetal::cambiaTipo(int n) {

	miTipoL = tipoIngredienteLetal(n);

	setTexture(texturaIngredienteLetal[n]);
}
