#include "Pared.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

Pared::Pared(Game* game, Vector2D<double> position) : Mueble(game, position, TILE_SIZE, TILE_SIZE, "cartel")
{
	setDimension(50, 50);
	setColliderRect({ (int)getX(), (int)getY(), w, h});
	//Collider invisible
}

Pared::~Pared()
{
}



void Pared::update()
{
	//for (auto i : game->getObjectManager()->getPoolGrupoClientes()->getCollisions(getCollider())) {
	//	i->colisionClientes();
	//}
}

void Pared::render(SDL_Rect* cameraRect)
{
}

//bool Pared::colisionPlayer(Player* p)
//{
//	p->setVel(0, 0);
//	return false;
//}
