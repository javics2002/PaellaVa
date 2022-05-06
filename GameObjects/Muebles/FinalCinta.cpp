#include "FinalCinta.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"
#include "../Ingrediente.h"

FinalCinta::FinalCinta(Game* mGame, Vector2D<double> pos) : Mueble(mGame, pos, TILE_SIZE, 2 * TILE_SIZE, "basura")
{
	//Siempre tiene que funcionar
	funcionando = true;
}

void FinalCinta::update()
{
	if (isActive()) {
		vector<Ingrediente*> ingredientes = mGame->getObjectManager()->getPool<Ingrediente>(_p_INGREDIENTE)->getCollisions(getCollider());
		vector<Ingrediente*> ingredientesletales = mGame->getObjectManager()->getPool<Ingrediente>(_p_INGREDIENTELETAL)->getCollisions(getCollider());
		for (auto i : ingredientes) {
			if (!i->isPicked())
				i->setActive(false);
		}
		for (auto i : ingredientesletales) {
			if (!i->isPicked())
				i->setActive(false);
		}
	}

}

bool FinalCinta::receiveIngrediente(Ingrediente* ingr)
{
	ingr->setActive(false);

	sdlutils().soundEffects().at("tirarBasura" + to_string(sdlutils().rand().nextInt(1, 3))).play();

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
	
	return true;
}

bool FinalCinta::receiveArroz(Arroz* arr)
{
	arr->setActive(false);

	return true;
}

