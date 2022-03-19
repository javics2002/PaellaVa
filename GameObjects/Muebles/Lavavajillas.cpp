#include "Lavavajillas.h"

Lavavajillas::Lavavajillas(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, TILE_SIZE, "lavavajillas")
{

}

void Lavavajillas::onCollision()
{
	if (!LavavajillasLleno())
		paellas++;
}

bool Lavavajillas::LavavajillasLleno()
{
	return paellas==MAX_PAELLERAS;
}

void Lavavajillas::metePaellera(Paella* paella)
{
	paelleras.push_back(paella);
}

void Lavavajillas::sacaPaellera(vector<Paella*>::iterator it)
{
	paelleras.erase(it);
}

void Lavavajillas::lavando()
{
	if (tiempo == 0.0) tiempo = sdlutils().currRealTime();

	if (sdlutils().currRealTime() - tiempo >= TIEMPO_LAVADO) {
		for (int i = 0; i < paelleras.size(); i++) {
			paelleras[i]->setLavado(Contenido::Limpia);
		}
	}
}

