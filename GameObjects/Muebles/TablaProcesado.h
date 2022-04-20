#pragma once
#include "Mueble.h"
#include "../Ingrediente.h"

class Game;

class TablaProcesado: public Mueble
{
	const unsigned int DIMENSION_W = 1;
	const unsigned int DIMENSION_H = 1;

	const int MAX_BREAK_TIME = 8000;
	const int REDUCE_BREAK_TIME = 500;
	int couldBreak = MAX_BREAK_TIME;
	int seg = 10;

	const double TIEMPO_PROCESADO = 3000.0;

	Ingrediente* ingr_;

	double tiempo = 0.0;

	int i = 0;

	SDL_Rect clip;

public:
	TablaProcesado(Game* game_, Vector2D<double> pos);
	~TablaProcesado() = default;

	void update() override;
	void procesando();
	void render(SDL_Rect *camera) override;

	bool receiveIngrediente(Ingrediente *ingr_) override;
	bool returnObject(Player* p);
};

