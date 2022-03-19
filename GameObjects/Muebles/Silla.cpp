#include "Silla.h"

#include "Mesa.h"

Silla::Silla(Game* game, Vector2D<double> pos, string texture) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, texture)
{
	mMesa = nullptr;
}

Silla* Silla::initMesa(Mesa* mesa)
{
	mMesa = mesa;

	return this;
}

bool Silla::recieveGrupoClientes(GrupoClientes* gc)
{
	return mMesa->recieveGrupoClientes(gc);
}

bool Silla::returnObject(Player* p)
{
	return mMesa->returnObject(p);
}

