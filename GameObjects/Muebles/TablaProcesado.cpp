#include "TablaProcesado.h"

TablaProcesado::TablaProcesado(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "tablaProcesado")
{
}

void TablaProcesado::onCollision()
{
	procesando();
	tablaLLena = true;
}

bool TablaProcesado::getTablaLlena()
{
	return tablaLLena;
}

void TablaProcesado::procesando()
{
	if (tiempo == 0.0) {
		tiempo = sdlutils().currRealTime();
	}

	if (sdlutils().currRealTime() - tiempo >= TIEMPO_PROCESADO) {
		ingr->setProcesado(true);
	}
}
