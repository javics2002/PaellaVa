#pragma once
#include "../GameObject.h"

class Game;

class Mueble : public GameObject
{
public:
	Mueble(Game* game, Vector2D<int> pos);
	Mueble(Game* game, Vector2D<int> pos, int rot);
	~Mueble() = default;
};

