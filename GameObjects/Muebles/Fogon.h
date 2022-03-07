#pragma once
#include "Mueble.h"

class Game;

class Fogon : public Mueble {
public:
	Fogon(Game* game, Vector2D<double> pos);
	~Fogon() = default;
};

