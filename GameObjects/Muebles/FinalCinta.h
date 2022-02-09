#pragma once
#include "../GameObject.h"
#include "../Ingrediente.h"
#include "../../Data/Pool.h"

class Game;

class FinalCinta : public GameObject
{
	const unsigned int DIMENSION_W = 70;
	const unsigned int DIMENSION_H = 120;

	Pool<Ingrediente>* poolIngredientes;
public:
	FinalCinta(Game* game, Pool<Ingrediente>* pool);
	~FinalCinta() = default;

	void update() override;
};



