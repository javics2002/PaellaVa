#pragma once
#include "Mueble.h"
#include "../Ingrediente.h"
#include "../../Data/Pool.h"
#include "../ObjetoPortable.h"
#include "../Paella.h"

class Game;

class FinalCinta : public Mueble
{
public:
	FinalCinta(Game* game, Vector2D<double> pos);
	~FinalCinta() = default;

	void update() override;
	bool receiveIngrediente(Ingrediente* ingr) override;
	bool receivePaella(Paella* pa) override;
	bool receiveArroz(Arroz* arr) override;
};



