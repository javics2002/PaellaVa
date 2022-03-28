#pragma once
#include "Mueble.h"

class Game;

class Pared : public Mueble
{
public:
	Pared(Game* game, Vector2D<double> position);
	~Pared();
	void update() override;
	void render(SDL_Rect* cameraRect) override;
	//bool colisionPlayer(Player* p) override;
};

