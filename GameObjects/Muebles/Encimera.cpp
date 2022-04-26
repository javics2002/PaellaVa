#include "Encimera.h"
#include "../Ingrediente.h"
#include "../Herramienta.h"
#include "../Player.h"
#include "../Paella.h"
#include "../../Control/Game.h"
#include "../../Scenes/Tutorial.h"

Encimera::Encimera(Game* game, Vector2D<double> pos) : Mueble(game, pos, 1 * TILE_SIZE, 2 * TILE_SIZE, "encimera")
{
	//Siempre tiene que funcionar
	funcionando = true;
}

bool Encimera::receiveIngrediente(Ingrediente* ingr)
{
	//Si ya tiene objeto, no recoge objeto
	if (ingr_ == nullptr && paella_ == nullptr && arroz_ == nullptr)
	{
		ingr_ = ingr;

		ingr_->setPosition(getRectCenter(getOverlap()));

		return true;
	}

	else if (paella_ != nullptr) {

		if (paella_->ingrValido(ingr) && paella_->conArroz() && ingr->getProcesado()) {

			paella_->anadeIngr(ingr);
			if (dynamic_cast<Tutorial*>(game->getCurrentScene()) && game->getCurrentScene()->getState()==States::dejarIngredientePaella) {
				if (paella_->ingredientesEnPaella() == 3)
					game->getCurrentScene()->changeState(States::pausaDejarIngredientes);
			}
			ingr->setActive(false);

			return true;
		}
	}

	return false;
}

bool Encimera::receivePaella(Paella* pa)
{
	//Si ya tiene objeto, no recoge objeto
	if (ingr_ == nullptr && paella_ == nullptr && arroz_ == nullptr)
	{
		if (dynamic_cast<Tutorial*>(game->getCurrentScene()) && game->getCurrentScene()->getState() == States::dejaPaellera)
			game->getCurrentScene()->changeState(States::pausaDejaPaella);


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
			if (dynamic_cast<Tutorial*>(game->getCurrentScene()))
				game->getCurrentScene()->changeState(States::pausaCogerDejarArroz);

			return true;
		}
		else
		{
			return false;
		}
	}
	else if (ingr_ == nullptr && arroz_ == nullptr && paella_==nullptr) {

		arroz_ = arr;
		arroz_->setPosition(getRectCenter(getOverlap()));

		return true;
	}

	return false;
}

bool Encimera::receiveHerramienta(Herramienta* h)
{
	//Si ya tiene objeto, no recoge objeto
	if (ingr_ == nullptr && paella_ == nullptr && arroz_ == nullptr && h_ == nullptr)
	{
		h_ = h;

		h_->setPosition(getRectCenter(getOverlap()));

		return true;
	}

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
	else if (arroz_ != nullptr)
	{
		p->setPickedObject(arroz_, ARROZ);

		arroz_ = nullptr;

		return true;
	}
	else if (h_ != nullptr)
	{
		p->setPickedObject(h_, HERRAMIENTA);

		h_ = nullptr;

		return true;
	}
	else
		return false;
}

