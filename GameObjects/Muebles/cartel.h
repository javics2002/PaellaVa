#pragma once
#include "Mueble.h"

class Game;

class Cartel : public Mueble
{
public:
	Cartel(Game* game, Vector2D<double> position);
	~Cartel() = default;

	void update() override;

	SDL_Rect getCollider() override;
};

