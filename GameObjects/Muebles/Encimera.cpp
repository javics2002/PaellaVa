#include "Encimera.h"
#include "../Ingrediente.h"
#include "../Player.h"
#include "../Paella.h"
#include "../../Control/Game.h"

Encimera::Encimera(Game* game, Vector2D<double> pos) : Mueble(game, pos, 1 * TILE_SIZE, 2 * TILE_SIZE, "encimera")
{
}

bool Encimera::receiveIngrediente(Ingrediente* ingr)
{
	//Si ya tiene objeto, no recoge objeto
	if (ingr_ == nullptr && paella_ == nullptr)
	{
		ingr_ = ingr;

		ingr_->setPosition(getRectCenter(getOverlap()));

		return true;
	}
	
	else if (paella_ != nullptr) {

		if (paella_->ingrValido(ingr) && paella_->conArroz() && ingr->getProcesado()) {

			paella_->anadeIngr(ingr);

			ingr->setActive(false);

			return true;
		}
	}

	return false;
}

bool Encimera::receivePaella(Paella* pa)
{
	//Si ya tiene objeto, no recoge objeto
	if (ingr_ == nullptr && paella_ == nullptr)
	{
		paella_ = pa;

		paella_->setPosition(getRectCenter(getOverlap()));

		return true;
	}
	else
		return false;
}

bool Encimera::receiveArroz(Arroz* arr)
{
	//Si ya tiene objeto, no recoge objeto
	if (paella_ != nullptr && paella_->getContenido() == Limpia)
	{
		if (paella_->getContenido() == Limpia) {

			paella_->anadeArroz(arr);

			return true;
		}
		else
		{
			return false;
		}		
	}
	
	else if (ingr_ == nullptr && arroz == nullptr) {

		arroz = arr;
		arroz->setPosition(getRectCenter(getOverlap()));

		return true;
	}

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
	else if (paella_ != nullptr)
	{
		p->setPickedObject(paella_, PAELLA);

		paella_ = nullptr;

		return true;
	}
	else if (arroz != nullptr)
	{
		p->setPickedObject(arroz, ARROZ);

		arroz = nullptr;

		return true;
	}
	else
		return false;
}
