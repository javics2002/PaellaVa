#pragma once
#include "Mueble.h"

class Game;

class Cinta : public Mueble
{
	Cinta(Game* game, Vector2D<int> pos);
	~Cinta() = default;
};

