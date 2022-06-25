#include "CajaTakeaway.h"
#include "../Control/Game.h"


CajaTakeaway::CajaTakeaway(Game* mGame) : ObjetoPortable(mGame) {
	setDimension(DIMENSION, DIMENSION);
	setTexture("cajaTakeaway");
	setDepth(2);
}

void CajaTakeaway::addIngreds(list<tipoIngrediente> ingred)
{
	ingreds_ = ingred;
}

list<tipoIngrediente> CajaTakeaway::getIngreds()
{
	return ingreds_;
}

void CajaTakeaway::resetCaja()
{
	ingreds_.clear();
	contaminada_ = false;
	cocinada_ = Cruda;

}

