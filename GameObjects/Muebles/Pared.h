#pragma once
#include "Mueble.h"

class Game;

class Pared : public Mueble
{
public:
	Pared(Game* mGame, Vector2D<double> position);
	~Pared();

	void render(SDL_Rect* cameraRect) override {};

	SDL_Rect getCollider() override;
	SDL_Rect getOverlap() override;

};

