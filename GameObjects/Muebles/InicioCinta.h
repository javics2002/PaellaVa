#pragma once
#include "Mueble.h"
#include "../Ingrediente.h"
#include "../Ingredienteletal.h"
#include "../../Data/Pool.h"
#include "../../Utils/ParticleExample.h"

class Game;

class InicioCinta : public Mueble
{
	const double SPAWN_DELAY = 2000;
	double initTime;

	const int MAX_BREAK_TIME = 18000;
	const int REDUCE_BREAK_TIME = 500;
	int couldBreak = MAX_BREAK_TIME;
	int seg = 10;

	int porcentajeLetal = 120; //numeros aleatorios entre 1 y 100 el x% que quieras sonn menores qeu esto

	int idCont = 0; // Asigna las IDs

public:
	InicioCinta(Game* mGame, Vector2D<double> pos);
	~InicioCinta() = default;

	void update() override;

	void render(SDL_Rect* cameraRect)override;

	SDL_Rect getOverlap() override;
	SDL_Rect getCollider() override;
	virtual bool resetCounter() override;

	void dejaFuncionar() { funcionando = false; };

	bool receiveHerramienta(Herramienta* h) override;
};

