#pragma once
#include "../GameObject.h"

constexpr auto TILE_SIZE = 64;

class Game;

class Mueble : public GameObject
{
public:
	Mueble(Game* game, Vector2D<double> position, int width, int height, string claveTextura);
	~Mueble() = default;
};



