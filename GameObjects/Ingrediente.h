#pragma once
#include "GameObject.h"

class Game;

class Ingrediente : public GameObject
{
	bool active;
public:
	Ingrediente(Game* game) : active(false), GameObject(game) {};
	~Ingrediente() {};

	void activate();
	void descativate();
	bool isActive();

	void update() override {};
};

