#pragma once
#include "Mueble.h"
#include "../Ingrediente.h"
#include "../../Data/Pool.h"

class Game;

class InicioCinta : public Mueble
{
	const double SPAWN_DELAY = 2000;

	double time;
public:
	InicioCinta(Game* game, Vector2D<double> pos);
	~InicioCinta() = default;

	void update() override;
};

