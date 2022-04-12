#pragma once
#include "Mueble.h"

class Game;

class Cinta : public Mueble
{
	SDL_Rect cintaCollider;

	SDL_Rect clip;
	int frameCounter;
	float lastFrameTime;
	float frameRate;

public:
	Cinta(Game* game, Vector2D<double> pos);
	~Cinta();

	SDL_Rect getCollider() override;
	void setCollider(SDL_Rect r) override;
	SDL_Rect getOverlap() override;

	void update() override;
	void render(SDL_Rect* cameraRect) override;
	void animUpdate();
};

