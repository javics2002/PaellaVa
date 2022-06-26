#include "cartel.h"
#include "../../Control/game.h"
#include "../../Control/ObjectManager.h"
#include "../Repartidor.h"

Cartel::Cartel(Game* mGame, Vector2D<double> position) : Mueble(mGame, position, TILE_SIZE, TILE_SIZE * 1.8, "cartel")
{
	//Siempre tiene que funcionar
	funcionando = true;
}

void Cartel::update()
{
	for (auto i : mGame->getObjectManager()->getPool<GrupoClientes>(_p_GRUPO)->getCollisions(getCollider())) 
		i->colisionClientes();	

	for (auto i : mGame->getObjectManager()->getPool<Repartidor>(_p_REPARTIDOR)->getCollisions(getCollider()))
		i->colisionCartel();
}

SDL_Rect Cartel::getOverlap()
{
	if (isActive())return getTexBox();
	return { 0,0,0,0 };
}

SDL_Rect Cartel::getCollider()
{
	SDL_Rect rect = getTexBox();

	if (isActive())return {
		rect.x,
		rect.y + 3 * rect.h / 4,
		rect.w,
		rect.h / 4
	};
	return { 0,0,0,0 };
}
