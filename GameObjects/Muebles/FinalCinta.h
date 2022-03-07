#pragma once
#include "Mueble.h"
#include "../Ingrediente.h"
#include "../../Data/Pool.h"

class Game;

class FinalCinta : public Mueble
{
public:
	FinalCinta(Game* game, Vector2D<double> pos);
	~FinalCinta() = default;

	void update() override;
};



