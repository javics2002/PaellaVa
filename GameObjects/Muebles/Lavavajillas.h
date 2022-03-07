#pragma once
#include "Mueble.h"

class Game;

class Lavavajillas : public Mueble
{
public:
	Lavavajillas(Game* game, Vector2D<double> pos);
	~Lavavajillas() = default;
};

