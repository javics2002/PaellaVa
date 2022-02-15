#pragma once

#include "GameObjects/GameObject.h"
#include "../../Data/Pool.h"
#include "GameObjects/Cliente.h"
#include "GameObjects/Cola.h"
#include "GameObjects/GrupoClientes.h"

class Game;

class Puerta : public GameObject
{
	const unsigned int DIMENSION_W = 70;
	const unsigned int DIMENSION_H = 120;

	const unsigned int maxGrupo = 6;

	const double SPAWN_DELAY = 2000;
	double time;

	Pool<Cliente>* poolClientes;

	Cola* cola;

public:
	Puerta(Game* game, Pool<Cliente>* pool);
	~Puerta();

	void update() override;
};




