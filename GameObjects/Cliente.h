#pragma once

#include "../GameObjects/PoolObject.h"

using namespace std;

class Game;

class Cliente : public PoolObject
{
	const unsigned int DIMENSION = 70;


public:
	Cliente(Game* game);
	~Cliente() = default;

	void update() override;

	void cambiaTextura(string textureN);

	void clienteRecogido();

protected:
	void onObjectPicked() override {};
	void onObjectDropped() override {};
};


