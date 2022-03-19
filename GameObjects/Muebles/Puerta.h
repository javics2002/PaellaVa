#pragma once

#include "Mueble.h"

#include "../../Data/Pool.h"
#include "../Cola.h"

class Game;

class Puerta : public Mueble
{
	const unsigned int MAX_TAM = 3;

	const double SPAWN_DELAY = 8000;
	double time;

	Cola* cola;

	vector<string> texturasClientes = { "alcachofa", "calamar", "cangrejo",
	"gamba", "langosta", "mejillon", "pimientoRojo", "pimientoVerde", "pollo"
	};

public:
	Puerta(Game* game, Vector2D<double> pos);
	~Puerta() = default;

	void update() override;
};









