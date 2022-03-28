#pragma once

#include "GameObject.h"

class Imagen : public GameObject
{
public:
	Imagen(Game* game);
	Imagen(Game* game, Vector2D<double> pos, int w, int h, string clave);
	Imagen(Game* game, double posX, double posY, int w, int h, string clave);
	~Imagen() {};
};

