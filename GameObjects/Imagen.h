#pragma once

#include "GameObject.h"

class Imagen : public GameObject
{
public:
	Imagen(Game* mGame);
	Imagen(Game* mGame, Vector2D<double> pos, int w, int h, string clave);
	Imagen(Game* mGame, double posX, double posY, int w, int h, string clave);
	~Imagen() {};
};

