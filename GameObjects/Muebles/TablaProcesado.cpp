#include "TablaProcesado.h"
#include "../../Control/Game.h"

TablaProcesado::TablaProcesado(Game* game_, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "tablaProcesado")
{
}

void TablaProcesado::update() {

	if (ingr != nullptr) {

		if (ingr->isPicked()) {
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
		ingr->setProcesado(true, ingr);
		game->procesandoIngr(1.2);
		tiempo = 0;
	}
}

bool TablaProcesado::recieveIngrediente(Ingrediente* ingr_)
{
	
	if (ingr == nullptr) {

		ingr_->setPosition(getX(), getY());

		ingr = ingr_;

		game->procesandoIngr(0);

		return true;
	}

	return false;

}
