#include "Fondo.h"

Fondo::Fondo(Game* game) : GameObject (game)
{
	setPosition(sdlutils().width(), sdlutils().height() / 2);
}
