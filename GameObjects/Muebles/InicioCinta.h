#pragma once
#include "Mueble.h"
#include "../Ingrediente.h"
#include "../Ingredienteletal.h"
#include "../../Data/Pool.h"

class Game;

class InicioCinta : public Mueble
{
	const double SPAWN_DELAY = 2000;
	double initTime;
	int porcentaeletal = 70;//numeros aleatorios entre 1 y 100 el x% que quieras sonn menores qeu esto

	bool host = true;

public:
	InicioCinta(Game* game, Vector2D<double> pos, bool host_);
	~InicioCinta() = default;

	void update() override;


	SDL_Rect getOverlap() override;
	SDL_Rect getCollider() override;
};

