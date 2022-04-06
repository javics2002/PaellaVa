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
	cout << "hola" << endl;

	if (!paellas.empty())
	{
		Paella* pa = paellas.front();

		cout << pa->getTexCollider().w << " " << pa->getTexCollider().h << " " << pa->getTexCollider().x << " " << pa->getTexCollider().y << endl;

		//pa->setPicked(true);
		//TOCHECK: Podríamos hacer un return del objeto y que el player se lo guarde a sí mismo
		p->setPickedObject(pa, PAELLA);

		paellas.pop_front();

		return true;
	}
	
	return false;
}

bool Pila::receivePaella(Paella* pa)
{
	//if (miTipo == pa->getTipo() && pa->getContenido() == Limpia) {

	//	paellas.push_back(pa);

	//	return true;
	//}

	return false;
}

void Pila::init(ObjectManager* objectManager)
{
	for (int i = 0; i < numPaellas; i++) 
		paellas.push_back(objectManager->addPaella(miTipo));
}
