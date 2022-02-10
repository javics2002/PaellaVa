#include "FinalCinta.h"
#include "../../Control/Game.h"

FinalCinta::FinalCinta(Game* game, Pool<Ingrediente>* pool) : GameObject(game), poolIngredientes(pool)
{
	setPosition(game->getWindowWidth() - DIMENSION_W / 2, DIMENSION_H / 2);
	setDimension(DIMENSION_W, DIMENSION_H);

	textureName = finalCintaTexture;
}

void FinalCinta::update()
{
	vector<Collider*> ingredientes = poolIngredientes->getCollisions(getCollider());
	for (auto i : ingredientes)
		poolIngredientes->erase(i->ingredientCollide());
}
