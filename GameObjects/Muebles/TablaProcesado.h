#pragma once
#include "Mueble.h"
#include "../Ingrediente.h"

class Game;

class TablaProcesado: public Mueble
{
	const unsigned int DIMENSION_W = 1;
	const unsigned int DIMENSION_H = 1;

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

