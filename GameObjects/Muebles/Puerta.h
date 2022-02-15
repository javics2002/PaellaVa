#pragma once

#include "../GameObject.h"
#include "../../Data/Pool.h"
#include "../Cliente.h"
#include "../Cola.h"
#include "../GrupoClientes.h"

class Game;

class Puerta : public GameObject
{
	const unsigned int DIMENSION_W = 70;
	const unsigned int DIMENSION_H = 120;

	const unsigned int maxGrupo = 6;

	const double SPAWN_DELAY = 4000;
	double time;

	Pool<Cliente>* poolClientes;

	Cola* cola;

public:
	Puerta(Game* game, Pool<Cliente>* pool);
	~Puerta() = default;

	void update() override;
};




