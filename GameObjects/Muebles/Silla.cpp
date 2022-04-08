#include "Silla.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

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

bool Silla::receiveGrupoClientes(GrupoClientes* gc)
{
	if (mMesa != nullptr) {
		return mMesa->receiveGrupoClientes(gc);
	}
}

bool Silla::receivePaella(Paella* paella)
{
	if (mMesa != nullptr) {
		return mMesa->receivePaella(paella);
	}	
}

bool Silla::returnObject(Player* p)
{
	if (mMesa != nullptr) {
		return mMesa->returnObject(p);
	}
}

bool Silla::colisionPlayer(Player* p)
{
	if (mMesa != nullptr) {
		return mMesa->colisionPlayer(p);
	}
}

