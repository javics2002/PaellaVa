#pragma once
#include "Mueble.h"

class Game;

class Pared : public GameObject
{
	Game* game_ = nullptr;
public:
	Pared(Game* game, Vector2D<double> position);
	~Pared();
	void update() override;
	bool colisionPlayer(Player* p) override;
};

