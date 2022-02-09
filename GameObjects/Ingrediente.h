#pragma once
#include "GameObject.h"

class Game;

class Ingrediente : public GameObject
{
	bool active;
	const unsigned int DIMENSION = 40;
public:
	Ingrediente(Game* game);
	~Ingrediente() = default;

	void activate();
	void descativate();
	bool isActive();

	void update() override;	
	bool ingredientsCollide() override;
};

