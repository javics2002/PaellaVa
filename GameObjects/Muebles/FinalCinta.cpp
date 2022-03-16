#include "FinalCinta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

FinalCinta::FinalCinta(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "basura")
{
}

void FinalCinta::update()
{
	vector<Collider*> ingredientes = game->getObjectManager()->getPoolIngredientes()->getCollisions(getCollider());
	for (auto i : ingredientes)
		game->getObjectManager()->getPoolIngredientes()->remove(i->ingredientCollide());
}
