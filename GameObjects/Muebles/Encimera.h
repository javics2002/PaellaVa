#pragma once
#include "Mueble.h"

class Encimera : public Mueble
{
public:
	Encimera(Game* game, Vector2D<double> pos);
	~Encimera() = default;
};

