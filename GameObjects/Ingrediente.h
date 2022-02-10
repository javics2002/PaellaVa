#pragma once
#include "PoolObject.h"

class Game;

class Ingrediente : public PoolObject
{
	const unsigned int DIMENSION = 40;
public:
	Ingrediente(Game* game);
	~Ingrediente() = default;

	void update() override;	
	std::list<PoolObject*>::const_iterator ingredientCollide() override;
};

