#include "Encimera.h"
#include "../Ingrediente.h"
#include "../Herramienta.h"
#include "../CajaTakeaway.h"
#include "../Player.h"
#include "../Paella.h"
#include "../../Control/Game.h"
#include "../../Scenes/Tutorial.h"

Encimera::Encimera(Game* mGame, Vector2D<double> pos) : Mueble(mGame, pos, 1 * TILE_SIZE, 2 * TILE_SIZE, "encimera"), cajaTakeaway_(nullptr)
{
	//Siempre tiene que funcionar
	funcionando = true;
}

bool Encimera::receiveIngrediente(Ingrediente* ingr)
{
	if (ingr != nullptr) {
		//Si ya tiene objeto, no recoge objeto
		if (paella_ != nullptr) {
			if (paella_->ingrValido(ingr) && paella_->conArroz() && ingr->getProcesado()) {

				paella_->anadeIngr(ingr);
				if (dynamic_cast<Tutorial*>(mGame->getCurrentScene()) && mGame->getCurrentScene()->getState() == States::TUTORIALSTATE_DEJAR_INGREDIENTE) {
					if (paella_->ingredientesEnPaella() == 3)
						mGame->getCurrentScene()->changeState(States::TUTORIALSTATE_PAUSA_DEJAR_INGREDIENTE);
				}
				ingr->setActive(false);

				return true;
			}

			return false;
		}


		if (ingr_ == nullptr && arroz_ == nullptr && herramienta_ == nullptr && cajaTakeaway_ == nullptr)
		{
			ingr_ = ingr;

			ingr_->setPosition(getRectCenter(getOverlap()));

			return true;
		}
	}
	return false;
}

bool Encimera::receivePaella(Paella* pa)
{
	if (pa != nullptr) {
		//Si ya tiene objeto, no recoge objeto
		if (ingr_ == nullptr && paella_ == nullptr && arroz_ == nullptr && herramienta_ == nullptr && cajaTakeaway_ == nullptr)
		{
			sdlutils().soundEffects().at("paellaMesa").play(0, mGame->UI);
			if (dynamic_cast<Tutorial*>(mGame->getCurrentScene()) && mGame->getCurrentScene()->getState() == States::TUTORIALSTATE_DEJA_PAELLA)
				mGame->getCurrentScene()->changeState(States::TUTORIALSTATE_PAUSA_DEJA_PAELLA);


			paella_ = pa;

			paella_->setPosition(getRectCenter(getOverlap()));

			return true;
		}

		// Tirar a caja
		else if (cajaTakeaway_ != nullptr) {

			cajaTakeaway_->addIngreds(pa->getVIngredientes());
			cajaTakeaway_->setContaminada(pa->estaContaminada());
			cajaTakeaway_->setCocinada(static_cast<Resultado>(pa->getCoccion()));
			cajaTakeaway_->setTexture("cajaTakeawayCerrada");

			pa->setTexture("paellaSucia");
			pa->setState(Preparacion);
			pa->setContenido(Sucia);
			pa->setEnsuciada();

			sdlutils().soundEffects().at("tirarPaella").play();

			return true;
		}
	}
	return false;
}

bool Encimera::receiveArroz(Arroz* arr)
{
	if (arr != nullptr) {
		//Si ya tiene objeto, no recoge objeto
		if (paella_ != nullptr)
		{
			if (paella_->getContenido() == Limpia) {

				paella_->anadeArroz(arr);
				if (dynamic_cast<Tutorial*>(mGame->getCurrentScene()))
					mGame->getCurrentScene()->changeState(States::TUTORIALSTATE_PAUSA_COGER_DEJAR_ARROZ);

				return true;
			}

			return false;
		}

		if (ingr_ == nullptr && arroz_ == nullptr && herramienta_ == nullptr && cajaTakeaway_ == nullptr) {

			arroz_ = arr;
			arroz_->setPosition(getRectCenter(getOverlap()));

			return true;
		}
	}
	
	return false;
}

bool Encimera::receiveHerramienta(Herramienta* h)
{
	if (h != nullptr) {
		//Si ya tiene objeto, no recoge objeto
		if (ingr_ == nullptr && paella_ == nullptr && arroz_ == nullptr && herramienta_ == nullptr && cajaTakeaway_ == nullptr)
		{
			herramienta_ = h;
			herramienta_->setPosition(getRectCenter(getOverlap()));

			return true;
		}
	}

	return false;
}

bool Encimera::receiveCajaTakeaway(CajaTakeaway* caja)
{
	if (caja == nullptr)
		return false;

	//Si ya tiene objeto, no recoge objeto
	if (ingr_ == nullptr && paella_ == nullptr && arroz_ == nullptr && herramienta_ == nullptr && cajaTakeaway_ == nullptr)
	{
		cajaTakeaway_ = caja;
		cajaTakeaway_->setPosition(getRectCenter(getOverlap()));

		return true;
	}

	return false;
}

bool Encimera::returnObject(Player* p)
{
	if ((p->getPickedPaellasCount() > 0 && paella_ == nullptr) || p->getPickedPaellasCount() >= p->getMaxPickedPaellasCount() || (p->getObjectType() != PAELLA && p->getPickedObject() != nullptr))
		return false;

	if (ingr_ != nullptr)
	{
		p->setPickedObject(ingr_, INGREDIENTE);
		ingr_ = nullptr;

		cout << "Objeto devuelto por: " << id << endl;

		return true;
	}
	else if (paella_ != nullptr)
	{
		p->setPickedObject(paella_, PAELLA);
		paella_ = nullptr;

		cout << "Objeto devuelto por: " << id << endl;

		return true;
	}
	else if (arroz_ != nullptr)
	{
		p->setPickedObject(arroz_, ARROZ);
		arroz_ = nullptr;

		cout << "Objeto devuelto por: " << id << endl;

		return true;
	}
	else if (herramienta_ != nullptr)
	{
		p->setPickedObject(herramienta_, HERRAMIENTA);
		herramienta_ = nullptr;

		cout << "Objeto devuelto por: " << id << endl;

		return true;
	}
	else if (cajaTakeaway_ != nullptr)
	{
		p->setPickedObject(cajaTakeaway_, CAJATAKEAWAY);
		cajaTakeaway_ = nullptr;

		cout << "Objeto devuelto por: " << id << endl;

		return true;
	}

	

	return false;
}

