#pragma once

#include "../GameObjects/PoolObject.h"

class Game;

class Cliente : public PoolObject
{
	const unsigned int DIMENSION = 70;

	enum { SENTADO, COMIENDO, ENCOLA, COGIDO ,CAMINANDO} estado;



public:
	Cliente(Game* game);	
	~Cliente() = default;

	void update() override;
	void onActivate() override;

	bool colisionClientes() override;

	void cambiaTextura(string nuevaClave);
};

