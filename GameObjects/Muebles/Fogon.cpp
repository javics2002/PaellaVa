#include "Fogon.h"
#include "../../Control/Game.h"
#include "../Paella.h"

Fogon::Fogon(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "fogon")
{
	paella_ = nullptr;
}

bool Fogon::receivePaella(Paella* pa)
{
	//Si ya tiene objeto, no recoge objeto
	if (paella_ == nullptr)
	{
		paella_ = pa;

		paella_->setPosition(getX(), getY());

		//empezar a cocer la paella
		paella_->setState(Coccion);

		return true;
	}
	else
		return false;
}

bool Fogon::returnObject(Player* p)
{
	if (paella_ != nullptr)
	{
		p->setPickedObject(paella_, PAELLA);

		paella_ = nullptr;

		//TOCHECK: Habría que devolver la paella al estado de "Preparación" si no está Preparada?
		//Y desde donde se llama a que esté ya Preparada?

		return true;
	}
	else
		return false;
}

