#pragma once

#include "ObjetoPortable.h"
#include <list>
#include "Paella.h"

class Game;

class CajaTakeaway : public ObjetoPortable
{
	const unsigned int DIMENSION = 60;

	bool contaminada_;
	Resultado cocinada_;
	list<tipoIngrediente> ingreds_;

public:
	CajaTakeaway(Game* mGame);
	~CajaTakeaway() = default;

	void onObjectPicked() override {};
	void onObjectDropped() override {};

	void setCocinada(Resultado coc) { cocinada_ = coc; };
	Resultado getCocinada() { return cocinada_; };

	void setContaminada(bool cont) { contaminada_ = cont; };
	bool getContaminada() { return contaminada_; };

	void addIngreds(list<tipoIngrediente> ingred);
	list<tipoIngrediente> getIngreds();
};
