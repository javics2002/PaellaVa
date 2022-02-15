#pragma once

#include "../GameObjects/PoolObject.h"

class Game;

class Cliente : public PoolObject
{
	const unsigned int DIMENSION = 70;

	enum { SENTADO, COMIENDO, DEPIE, COGIDO };

public:
	Cliente(Game* game);	
	~Cliente() = default;

	void update() override;
};

