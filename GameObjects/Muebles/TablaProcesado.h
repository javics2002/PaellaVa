#pragma once
#include "Mueble.h"

class Game;

class TablaProcesado: public Mueble
{
	const unsigned int DIMENSION_W = 1;
	const unsigned int DIMENSION_H = 1;
public:
	TablaProcesado(Game* game, Vector2D<double> pos);
	~TablaProcesado() = default;

	//void update();
	//void onCollision();
};

