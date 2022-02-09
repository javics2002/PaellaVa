#pragma once
#include "../GameObject.h"
#include "../Ingrediente.h"
#include "../../Data/Pool.h"

class Game;

class InicioCinta : public GameObject
{
	const unsigned int DIMENSION_W = 90;
	const unsigned int DIMENSION_H = 270;

	const double SPAWN_DELAY = 3000;
	double time;

	Pool<Ingrediente>* poolIngredientes;
public:
	InicioCinta(Game* game, Pool<Ingrediente>* pool);
	~InicioCinta() = default;

	void update() override;
};

