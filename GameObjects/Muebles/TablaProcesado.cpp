#include "TablaProcesado.h"

TablaProcesado::TablaProcesado(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "tablaProcesado")
{
}

void TablaProcesado::update() {

	if (ingr != nullptr && ingr->getCogido()) {
		tiempo = 0;
		tablaLLena = true;
	}

}

void TablaProcesado::onCollision()
{
	procesando();

}

bool TablaProcesado::getTablaLlena()
{
	return tablaLLena;
}

void TablaProcesado::procesando()
{
	if (tiempo == 0.0) {
		tiempo = sdlutils().currRealTime();
		tablaLLena = true;
		ingr->setCogido(false);
	}

	if (sdlutils().currRealTime() - tiempo >= TIEMPO_PROCESADO) {
		ingr->setProcesado(true);
		tiempo = 0;
	}
}
