#include "Mueble.h"
#include "../../Control/Game.h"

Mueble::Mueble(Game* game, Vector2D<int> pos) : GameObject(game)
{
	setPosition(pos.getX(), pos.getY());
}

Mueble::Mueble(Game* game, Vector2D<int> pos, int rot) : GameObject(game)
{
	setPosition(pos.getX(), pos.getY());

}
