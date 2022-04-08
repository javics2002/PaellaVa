#pragma once

#include "PoolObject.h"
#include "GameObject.h"

class Game;

class Cliente : public GameObject, public PoolObject
{
	const unsigned int DIMENSION = 70;


public:
	Cliente(Game* game);
	~Cliente() = default;

	void update() override;

	void cambiaTextura(string textureN);
};


