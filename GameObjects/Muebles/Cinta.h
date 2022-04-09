#pragma once
#include "Mueble.h"

class Game;

class Cinta : public Mueble
{
	SDL_Rect cintaCollider;
public:
	Cinta(Game* game, Vector2D<double> pos);
	~Cinta();

	SDL_Rect getCollider() override;
	void setCollider(SDL_Rect r) override;
	SDL_Rect getOverlap() override;
};

