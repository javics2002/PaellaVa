#include "Pila.h"

#include "../Player.h"
#include "../../GameObjects/Paella.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"

Pila::Pila(Game* game, Vector2D<double> pos, int miTipo_, int nPaellas) 
	: Mueble(game, pos, 1 * TILE_SIZE, 2 * TILE_SIZE, "berenjena")
{
	miTipo = miTipo_;
	numPaellas = nPaellas;

	paellas = deque<Paella*>();	
}

bool Pila::returnObject(Player* p)
{
	if (!paellas.empty())
	{
		Paella* pa = paellas.front();
		p->setPickedObject(pa, PAELLA);
		paellas.pop_front();

		return true;
	}
	
	return false;
}

bool Pila::receivePaella(Paella* pa)
{
	if (miTipo == pa->getTipo() && pa->getContenido() == Limpia) {

		paellas.push_back(pa);
		pa->setPosition(getPosition());

		return true;
	}

	return false;
}

void Pila::init(ObjectManager* objectManager)
{
	for (int i = 0; i < numPaellas; i++) {
		Paella* pa = objectManager->addPaella(miTipo);
		pa->setPosition(getPosition());
		paellas.push_back(pa);
	}		
}
