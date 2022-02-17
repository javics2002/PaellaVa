#include "cartel.h"
#include "../../Control/game.h"

Cartel::Cartel(Game* game) : GameObject(game)
{
	textureName = cartel;
	setDimension(DIMENSION_W, DIMENSION_H);
	setPosition(1000, game->getWindowHeight() - DIMENSION_H / 2);
}

void Cartel::update()
{
	for (auto i : game->getClientes(getCollider())) {
		i->colisionClientes();
	}
}
