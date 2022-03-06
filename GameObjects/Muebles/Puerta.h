#pragma once

#include "Mueble.h"

#include "../../Data/Pool.h"
#include "../Cola.h"

class Game;

class Puerta : public Mueble
{
	const unsigned int maxGrupo = 6;

	const double SPAWN_DELAY = 4000;
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









