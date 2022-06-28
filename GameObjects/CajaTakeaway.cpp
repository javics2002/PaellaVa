#include "CajaTakeaway.h"
#include "../Control/Game.h"


CajaTakeaway::CajaTakeaway(Game* mGame) : ObjetoPortable(mGame), llena_(false) {
	setDimension(DIMENSION, DIMENSION);
	setTexture("cajaTakeaway");
	setDepth(2);
}

void CajaTakeaway::addIngreds(list<tipoIngrediente> ingred)
{
	llena_ = true;
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
	llena_ = false;
	contaminada_ = false;
	cocinada_ = Cruda;

}

