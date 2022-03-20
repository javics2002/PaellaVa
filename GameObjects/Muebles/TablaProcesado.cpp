#include "TablaProcesado.h"
#include "../../Control/Game.h"

TablaProcesado::TablaProcesado(Game* game_, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "tablaProcesado")
{
}

void TablaProcesado::update() {

	if (ingr_ != nullptr) {

		if (ingr_->isPicked()) {
			tiempo = 0;
		}

		else procesando();
	}


}

void TablaProcesado::procesando()
{
	if (tiempo == 0.0) {
		tiempo = sdlutils().currRealTime();
	}

	if (sdlutils().currRealTime() - tiempo >= TIEMPO_PROCESADO) {
		ingr_->setProcesado(true, ingr_);
		game->procesandoIngr(1.2);
		tiempo = 0;
	}
}

bool TablaProcesado::receiveIngrediente(Ingrediente* ingr)
{
	
	if (ingr_ == nullptr) {

		ingr_ = ingr;

		ingr_->setPosition(getX(), getY());

		game->procesandoIngr(0);

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
