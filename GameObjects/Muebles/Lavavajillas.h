#pragma once
#include "Mueble.h"
#include "../Paella.h"

class Game;

class Lavavajillas : public Mueble
{
public:

	//um Contenido { Limpia, Entera, Mitad, Sucia };

	const unsigned int DIMENSION_W = 1;
	const unsigned int DIMENSION_H = 1;

	const double TIEMPO_LAVADO = 3000.0;

	double tiempo = 0.0;

	Paella* paella;

public:
	Lavavajillas(Game* game, Vector2D<double> pos);
	~Lavavajillas() = default;

	void update();
	void lavando();

	bool receivePaella(Paella* paella_) override;
	bool returnObject(Player* p);
};



