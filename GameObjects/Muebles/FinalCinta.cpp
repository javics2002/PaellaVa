#include "FinalCinta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"
#include "../Ingrediente.h"

FinalCinta::FinalCinta(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "basura")
{
	setColliderRect({ (int)getX(), (int)getY(), w, h});
}

void FinalCinta::update()
{
	vector<Collider*> ingredientes = game->getObjectManager()->getPoolIngredientes()->getCollisions(getCollider());
	for (auto i : ingredientes) {
		auto pair = i->colisionIngrediente();
		cout << "size: " << ingredientes.size() << endl;
		if (pair.first)
			game->getObjectManager()->getPoolIngredientes()->remove(pair.second);
	}	
}

bool FinalCinta::receiveIngrediente(Ingrediente* ingr)
{
	game->getObjectManager()->getPoolIngredientes()->remove(ingr->getIterator());

	return true;
}

bool FinalCinta::receivePaella(Paella* pa)
{
	//hacer cosas
	pa->setTexture("paellaSucia");
	pa->setContenido(Sucia);
	return false;
}

