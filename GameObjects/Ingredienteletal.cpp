#include "Ingredienteletal.h"
Ingredienteletal::Ingredienteletal(Game* game):Ingrediente(game)
{
	setDimension(DIMENSION, DIMENSION);
	setDepth(2);
	letal = true;
}
Ingredienteletal::~Ingredienteletal()
{
}
void Ingredienteletal::onActivate()
{
	int n = rand() % (tipoIngredienteletal::LEST); // Se le resta uno a LAST porque el arroz no debe salir en la
													  // cinta (siendo arroz = 0).


	miTipoL = tipoIngredienteletal(n);


	setTexture(texturaIngredienteletal[n]);
}
