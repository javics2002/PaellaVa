#pragma once
#include "Mueble.h"

class Ingrediente;

class Encimera : public Mueble
{
private:
	Ingrediente* in_ = nullptr;

public:
	Encimera(Game* game, Vector2D<double> pos);
	~Encimera() = default;

	virtual bool receiveIngrediente(Ingrediente* in) override;
	virtual bool returnObject(Player* p) override;
};

