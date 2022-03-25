#include "Pared.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

Pared::Pared(Game* game, Vector2D<double> position) : GameObject(game)
{
	game_ = game;
	setTexture("berenjena"); //Con esto no se rompe
	//Collider invisible
}

Pared::~Pared()
{
}

void Pared::update()
{
	for (auto i : game->getObjectManager()->getPoolGrupoClientes()->getCollisions(getCollider())) {
		i->colisionClientes();
	}
}

bool Pared::colisionPlayer(Player* p)
{
	p->setVel(0, 0);
	return false;
}
