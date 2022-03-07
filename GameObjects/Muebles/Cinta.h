#pragma once
#include "Mueble.h"

class Game;

class Cinta : public Mueble
{
public:
	Cinta(Game* game, Vector2D<double> pos);
	~Cinta() = default;
};

