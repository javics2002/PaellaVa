#include "Imagen.h"

Imagen::Imagen(Game* mGame) : GameObject (mGame)
{
}

Imagen::Imagen(Game* mGame, Vector2D<double> pos, int w, int h, string clave) : GameObject(mGame)
{
	setPosition(pos);
	setDimension(w, h);
	setInitialDimension(w, h);
	setTexture(clave);
}

Imagen::Imagen(Game* mGame, double posX, double posY, int w, int h, string clave) : GameObject(mGame)
{
	setInitialPosition(posX, posY);
	setPosition(posX, posY);
	setDimension(w, h);
	setInitialDimension(w, h);
	setTexture(clave);
}
