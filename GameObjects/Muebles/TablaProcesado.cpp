#include "TablaProcesado.h"

TablaProcesado::TablaProcesado(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "tablaProcesado")
{
}

void TablaProcesado::onCollision()
{
	ingr->procesando();
	tablaLLena = true;
}

bool TablaProcesado::getTablaLlena()
{
	return tablaLLena;
}
