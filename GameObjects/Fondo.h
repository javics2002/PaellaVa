#pragma once

#include "GameObject.h"

class Fondo : public GameObject
{
public:
	Fondo(Game* game, Texture* texture);
	~Fondo() {};
};

