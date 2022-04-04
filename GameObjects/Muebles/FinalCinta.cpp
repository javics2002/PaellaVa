#include "FinalCinta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"
#include "../Ingrediente.h"

FinalCinta::FinalCinta(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "basura")
{
	setColliderRect({ (int)getX(), (int)getY(), w, h });
}

void FinalCinta::update()
{
	vector<Collider*> ingredientes = game->getObjectManager()->getPoolIngredientes()->getCollisions(getCollider());
	for (auto i : ingredientes) {
		auto pair = dynamic_cast<Ingrediente*>(i)->colisionIngrediente();
		if (pair.first)
			game->getObjectManager()->getPoolIngredientes()->remove(pair.second);
	}	
}

bool FinalCinta::receiveIngrediente(Ingrediente* ingr)
{
	ingr->deactivate();

	return true;
}

bool FinalCinta::receivePaella(Paella* pa)
{
	//hacer cosas
	pa->setTexture("paellaSucia");
	pa->setContenido(Sucia);
	pa->setEnsuciada();
	return false;
}

bool FinalCinta::receiveArroz(Arroz* arr)
{
	arr->deactivate();

	return true;
}

