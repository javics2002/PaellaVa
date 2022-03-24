#include "TablaProcesado.h"
#include "../../GameObjects/Player.h"

TablaProcesado::TablaProcesado(Game* game_, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "tablaProcesado")
{
}

void TablaProcesado::update() {

	if (ingr_ != nullptr) {
		procesando();
	}


}

void TablaProcesado::procesando()
{
	if (sdlutils().currRealTime() - tiempo >= TIEMPO_PROCESADO) {
		ingr_->setProcesado(true, ingr_);
		tiempo = sdlutils().currRealTime();
	}
}

bool TablaProcesado::receiveIngrediente(Ingrediente* ingr)
{
	
	if (ingr_ == nullptr) {

		ingr_ = ingr;

		tiempo = sdlutils().currRealTime();

		ingr_->setPosition(getX(), getY());

		return true;
	}

	return false;

}

bool TablaProcesado::returnObject(Player* p)
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
