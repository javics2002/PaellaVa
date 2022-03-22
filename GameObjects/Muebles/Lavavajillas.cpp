#include "Lavavajillas.h"

Lavavajillas::Lavavajillas(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, TILE_SIZE, "lavavajillas")
{

}

void Lavavajillas::update()
{
	if (paella != nullptr)
		lavando();
}

void Lavavajillas::lavando()
{
	if (sdlutils().currRealTime() - tiempo >= TIEMPO_LAVADO) {
		paella->setLavado(Limpia,"alcachofa");
		tiempo = sdlutils().currRealTime();
	}
}

bool Lavavajillas::receivePaella(Paella* paella_)
{
	if (paella == nullptr) {

		paella = paella_;

		tiempo = sdlutils().currRealTime();

		paella->setPosition(getX(), getY());

		return true;
	}

	return false;
}

bool Lavavajillas::returnObject(Player* p)
{
	if (paella != nullptr)
	{
		//TOCHECK: Podríamos hacer un return del objeto y que el player se lo guarde a sí mismo
		p->setPickedObject(paella, PAELLA);

		paella = nullptr;

		return true;
	}
	else
		return false;
}

