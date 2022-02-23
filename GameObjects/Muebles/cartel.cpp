#include "cartel.h"
#include "../../Control/game.h"

Cartel::Cartel(Game* game) : GameObject(game)
{
	setTexture("cartel");
	setDimension(DIMENSION_W, DIMENSION_H);
	setPosition(1000, sdlutils().height() - DIMENSION_H / 2);
}

void Cartel::update()
{
	for (auto i : game->getObjectManager()->getClientes(getCollider())) {
		i->colisionClientes();
	}
}
