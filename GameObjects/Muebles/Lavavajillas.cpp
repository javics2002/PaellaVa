#include "Lavavajillas.h"

Lavavajillas::Lavavajillas(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, TILE_SIZE, "lavavajillas")
{

}

void Lavavajillas::update()
{
	if (!pilaPaellas.empty())
		lavando();
}

void Lavavajillas::lavando()
{

	if (sdlutils().currRealTime() - tiempo >= TIEMPO_LAVADO) {
		pilaPaellas.front()->setLavado(Limpia,"paellaLimpia");
		paellasLimpias.push_back(pilaPaellas.front());
		pilaPaellas.pop_front();
		tiempo = sdlutils().currRealTime();
	}
}

bool Lavavajillas::receivePaella(Paella* paella_)
{
	if (pilaPaellas.empty() && paella_->getContenido()==Sucia) {

		pilaPaellas.push_back(paella_);

		paella_->setPosition(getX(), getY());

		tiempo = sdlutils().currRealTime();

		return true;
	}

	return false;
}

bool Lavavajillas::returnObject(Player* p)
{
	if (!paellasLimpias.empty())
	{
		//TOCHECK: Podríamos hacer un return del objeto y que el player se lo guarde a sí mismo
		p->setPickedObject(paellasLimpias.front(), PAELLA);

		paellasLimpias.pop_front();

		return true;
	}
	else
		return false;
}

