#pragma once

#include "../GameObject.h"

class Game;

class Cartel : public GameObject
{
	const unsigned int DIMENSION_W = 70;
	const unsigned int DIMENSION_H = 120;


public:
	Cartel(Game* game);
	~Cartel() = default;

	void update() override;
};

