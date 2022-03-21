#include "FinalCinta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"
#include "../Ingrediente.h"

FinalCinta::FinalCinta(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "basura")
{
}

void FinalCinta::update()
{
	/*vector<Collider*> ingredientes = game->getObjectManager()->getPoolIngredientes()->getCollisions(getCollider());
	for (auto i : ingredientes)
		game->getObjectManager()->getPoolIngredientes()->remove(i->ingredientCollide());*/
}

void FinalCinta::interact(ObjetoPortable* o)
{
	if (dynamic_cast<Ingrediente*>(o)) {
		//desactivar ingrediente
		//o->remove
	}
	else if (dynamic_cast<Paella*>(o)) {

		/*o->eliminarIngr();
		o->changeTexture(clave);*/
	}
}


