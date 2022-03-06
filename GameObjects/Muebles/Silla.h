#pragma once
#include "Mueble.h"

class game;

class Silla : public Mueble
{
	const unsigned int DIMENSION_W = 1;
	const unsigned int DIMENSION_H = 1;
public:
	Silla(Game* game, Vector2D<double> pos, string texture);
	~Silla() = default;
};

