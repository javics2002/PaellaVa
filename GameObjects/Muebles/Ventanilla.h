#pragma once
#include "Mueble.h"

class Ventanilla : public Mueble
{
public:
	Ventanilla(Game* game, Vector2D<double> pos);
	~Ventanilla() = default;
};

