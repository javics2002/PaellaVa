#include "Encimera.h"
#include "../Ingrediente.h"
#include "../Player.h"

Encimera::Encimera(Game* game, Vector2D<double> pos) : Mueble(game, pos, 1 * TILE_SIZE, 2 * TILE_SIZE, "encimera")
{
}

bool Encimera::receiveIngrediente(Ingrediente* in)
{
	//Si ya tiene objeto, no recoge objeto
	if (in_ == nullptr) 
	{
		in->setPosition(getX(), getY());

		in_ = in;

		return true;
	}
	else
		return false;
}

bool Encimera::returnObject(Player* p)
{
	if (in_ != nullptr)
	{
		//TOCHECK: Podríamos hacer un return del objeto y que el player se lo guarde a sí mismo
		p->setPickedObject(in_, INGREDIENTE);

		in_ = nullptr;

		return true;
	}
	else
		return false;
}
