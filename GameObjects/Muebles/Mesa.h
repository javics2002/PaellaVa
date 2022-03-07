#pragma once
#include "Mueble.h"

class Game;

class Mesa : public Mueble
{
public:
	Mesa(Game* game, Vector2D<double> pos, Vector2D<int> dim, string texture);
};

