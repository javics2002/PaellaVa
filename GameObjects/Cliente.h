#pragma once

#include "GameObject.h"

class Game;

class Cliente:public GameObject
{
	enum { SENTADO, COMIENDO, DEPIE, COGIDO };

public:

	Cliente(Game* game) :GameObject(game) {};

	void Render();
};

