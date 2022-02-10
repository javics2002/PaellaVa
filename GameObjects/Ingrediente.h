#pragma once
#include "PoolObject.h"

#include <vector>

class Game;

class Ingrediente : public PoolObject
{
	const unsigned int DIMENSION = 70;

	int tipoIngrediente;

public:
	Ingrediente(Game* game);
	~Ingrediente() = default;

	void update() override;	
	std::list<PoolObject*>::const_iterator ingredientCollide() override;

	void onActivate() override;
};

