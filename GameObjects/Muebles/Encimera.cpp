#include "Encimera.h"
#include "../Ingrediente.h"
#include "../Player.h"
#include "../Paella.h"
#include "../../Control/Game.h"

Encimera::Encimera(Game* game, Vector2D<double> pos) : Mueble(game, pos, 1 * TILE_SIZE, 2 * TILE_SIZE, "encimera")
{
	setColliderRect({ (int)getX(), (int)getY() - h/ 3, w, 2*h / 3 });
}

bool Encimera::receiveIngrediente(Ingrediente* ingr)
{
	//Si ya tiene objeto, no recoge objeto
	if (ingr_ == nullptr && paella_ == nullptr)
	{
		ingr_ = ingr;

		ingr_->setPosition(getX(), getY() - ingr->getHeight()/2);

		return true;
	}
	
	else if (paella_ != nullptr) {

		if (paella_->ingrValido(ingr) && paella_->conArroz() && ingr->getProcesado()) {

			paella_->anadeIngr(ingr);

			game->getObjectManager()->getPoolIngredientes()->remove(ingr->getIterator());

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

		paella_->setPosition(getX(), getY() - 3 * pa->getHeight() / 4);

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
		paella_->anadeArroz(arr);

		return true;
	}
	else if (paella_ != nullptr && paella_->getContenido() != Limpia)
	{
		//no lo suelta
		return false;
	}
	else if (ingr_ == nullptr && arroz == nullptr) {
		arroz = arr;

		arroz->setPosition(getX(), getY() - arroz->getHeight() / 2);
		return true;
	}
	else
		return false;
}

bool Encimera::returnObject(Player* p)
{
	if (ingr_ != nullptr)
	{
		//TOCHECK: Podr�amos hacer un return del objeto y que el player se lo guarde a s� mismo
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
