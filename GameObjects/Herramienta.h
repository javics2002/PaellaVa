#pragma once

#include "ObjetoPortable.h"

class Game;

class Herramienta : public ObjetoPortable
{
	const unsigned int DIMENSION = 70;

public:
	Herramienta(Game* game);
	~Herramienta() = default;

	void onObjectPicked() override {};
	void onObjectDropped() override {};
};



