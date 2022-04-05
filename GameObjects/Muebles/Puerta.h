#pragma once

#include "Mueble.h"

#include "../../Data/Pool.h"
#include "../Cola.h"

class Game;

class Puerta : public Mueble
{
	const unsigned int MAX_TAM = 1;

	const double SPAWN_DELAY = 8000;

	Cola* cola;

	vector<string> texturasClientes = { "clienteBase"
	};

public:
	Puerta(Game* game, Vector2D<double> pos);
	~Puerta() = default;

	void update() override;
	virtual bool receiveGrupoClientes(GrupoClientes* gc);
};









