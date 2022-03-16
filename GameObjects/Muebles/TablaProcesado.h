#pragma once
#include "Mueble.h"
#include "../Ingrediente.h"

class Game;

class TablaProcesado: public Mueble
{
	const unsigned int DIMENSION_W = 1;
	const unsigned int DIMENSION_H = 1;

	const double TIEMPO_PROCESADO = 3000.0;

	Ingrediente* ingr;

	bool tablaLLena = false;

	double tiempo = 0.0;

public:
	TablaProcesado(Game* game, Vector2D<double> pos);
	~TablaProcesado() = default;

	void update();
	void onCollision();
	bool getTablaLlena();
	void procesando();
};

