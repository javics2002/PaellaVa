#include "Imagen.h"

Imagen::Imagen(Game* game) : GameObject (game)
{
}

Imagen::Imagen(Game* game, Vector2D<double> pos, int w, int h, string clave) : GameObject(game)
{
	setPosition(pos);
	setDimension(w, h);
	setTexture(clave);
}

Imagen::Imagen(Game* game, double posX, double posY, int w, int h, string clave) : GameObject(game)
{
	setPosition(posX, posY);
	setDimension(w, h);
	setTexture(clave);
}
