#include "Encimera.h"
#include "../Ingrediente.h"
#include "../Player.h"

Encimera::Encimera(Game* game, Vector2D<double> pos) : Mueble(game, pos, 1 * TILE_SIZE, 2 * TILE_SIZE, "encimera")
{
}

bool Encimera::receiveIngrediente(Ingrediente* ingr)
{
	//Si ya tiene objeto, no recoge objeto
	if (ingr_ == nullptr) 
	{
		ingr->setPosition(getX(), getY());

		ingr_ = ingr;

		return true;
	}
	else
		return false;
}

bool Encimera::returnObject(Player* p)
{
	if (ingr_ != nullptr)
	{
		//TOCHECK: Podríamos hacer un return del objeto y que el player se lo guarde a sí mismo
		p->setPickedObject(ingr_, INGREDIENTE);

		ingr_ = nullptr;

		return true;
	}
	else
		return false;
}
