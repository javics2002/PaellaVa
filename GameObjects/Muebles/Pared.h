#pragma once
#include "Mueble.h"

class Pared : public GameObject
{
public:
	Pared(Game* game, Vector2D<double> position);
	~Pared();
};

