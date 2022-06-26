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

bool CajaTakeaway::containsIngr(tipoIngrediente ingr)
{
	if (ingr == LAST)
		return true;

	auto it = ingreds_.begin();
	bool contains = false;
	while (it != ingreds_.end() && !contains) {

		if (*it == ingr) contains = true;
		it++;
	}

	return contains;
}

void CajaTakeaway::resetCaja()
{
	ingreds_.clear();
	contaminada_ = false;
	cocinada_ = Cruda;

}

