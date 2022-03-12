#pragma once
#include "Mueble.h"
#include "../Ingrediente.h"

class Game;

class TablaProcesado: public Mueble
{
	const unsigned int DIMENSION_W = 1;
	const unsigned int DIMENSION_H = 1;

	Ingrediente* ingr;

	bool tablaLLena = false;

public:
	TablaProcesado(Game* game, Vector2D<double> pos);
	~TablaProcesado() = default;

	//void update();
	void onCollision();
	bool getTablaLlena();
};

