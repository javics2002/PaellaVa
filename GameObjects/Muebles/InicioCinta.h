#pragma once
#include "../GameObject.h"
#include "../Ingrediente.h"
#include "../../Data/Pool.h"

class Game;

class InicioCinta : public GameObject
{
	const unsigned int DIMENSION_W = 70;
	const unsigned int DIMENSION_H = 120;

	const double SPAWN_DELAY = 2000;
	double time;

	Pool<Ingrediente>* poolIngredientes;
public:
	InicioCinta(Game* game, Pool<Ingrediente>* pool);
	~InicioCinta() = default;

	void update() override;
};

