#include "FinalCinta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"
#include "../Ingrediente.h"

FinalCinta::FinalCinta(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "basura")
{
}

void FinalCinta::update()
{
	vector<Ingrediente*> ingredientes = game->getObjectManager()->getPool<Ingrediente>(_p_INGREDIENTE)->getCollisions(getCollider());
	for (auto i : ingredientes) {
		if (!i->isPicked())
			i->deactivate();
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
	if (pa->getContenido() != Limpia) {
		pa->setTexture("paellaSucia");
		pa->setContenido(Sucia);
		pa->setEnsuciada();

		sdlutils().soundEffects().at("tirarPaella").play();
	}
	
	return false;
}

bool FinalCinta::receiveArroz(Arroz* arr)
{
	arr->deactivate();

	return true;
}

