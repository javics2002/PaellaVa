#include "Pila.h"

#include "../Player.h"
#include "../../GameObjects/Paella.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"
#include "../../Scenes/Tutorial.h"

Pila::Pila(Game* mGame, Vector2D<double> pos, int miTipo_, int nPaellas) 
	: Mueble(mGame, pos, 1 * TILE_SIZE, 2 * TILE_SIZE, "pilaPaellas")
{
	miTipo = miTipo_;
	numPaellas = nPaellas;

	paellas = deque<Paella*>();	

	//Siempre tiene que funcionar
	funcionando = true;
}

bool Pila::returnObject(Player* p)
{
	if (paellas.empty() || p->getPickedPaellasCount() >= p->getMaxPickedPaellasCount() || (p->getPickedPaellasCount() == 0 && p->getPickedObject() != nullptr))
		return false;

	if (dynamic_cast<Tutorial*>(mGame->getCurrentScene()) && mGame->getCurrentScene()->getState()==States::TUTORIALSTATE_COGE_PAELLA)
		mGame->getCurrentScene()->changeState(States::TUTORIALSTATE_PAUSA_COGE_PAELLA);
	Paella* pa = paellas.front();
	p->setPickedObject(pa, PAELLA);
	paellas.pop_front();
	switch (miTipo) {
	case Pequena:
		sdlutils().soundEffects().at("paellaPequena").play();
		break;
	case Mediana:
		sdlutils().soundEffects().at("paellaMediana").play();
		break;
	case Grande:
		sdlutils().soundEffects().at("paellaGrande").play();
		break;
	default:
		break;
	}
	return true;
}

bool Pila::receivePaella(Paella* pa)
{
	if (miTipo == pa->getTipo() && pa->getContenido() == Limpia) {

		paellas.push_back(pa);
		pa->setPosition(getRectCenter(getOverlap()));

		if (dynamic_cast<Tutorial*>(mGame->getCurrentScene()) && mGame->getCurrentScene()->getState() == States::TUTORIALSTATE_DEJAR_PAELLA_PILA)
			mGame->getCurrentScene()->changeState(States::TUTORIALSTATE_PAUSA_FINAL);

		return true;
	}

	return false;
}

void Pila::init(ObjectManager* objectManager)
{
	for (int i = 0; i < numPaellas; i++) {
		Paella* pa = objectManager->addPaella(miTipo);
		pa->setPosition(getRectCenter(getOverlap()));
		if (dynamic_cast<Tutorial*>(mGame->getCurrentScene()))
			pa->setActive(false);
		paellas.push_back(pa);
	}		
}
