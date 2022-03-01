#include "Fondo.h"

Fondo::Fondo(Game* game) : GameObject (game)
{
	setPosition(600, 500);
	setDimension(1080, 720);
}
