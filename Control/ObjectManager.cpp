#include "ObjectManager.h"
#include "../GameObjects/Muebles/InicioCinta.h"
#include "../GameObjects/Muebles/FinalCinta.h"
#include "../GameObjects/Muebles/Puerta.h"
#include  "../GameObjects/UI/RedactaComandabutton.h"
#include "../Data/Comanda.h"
#include "../GameObjects/Muebles/cartel.h"
#include "../Utils/Vector2D.h"
#include "../GameObjects/Ingrediente.h"
#include "../Scenes/Restaurante.h"

ObjectManager::ObjectManager(Game* game)
{
	ingredientes = new Pool<Ingrediente>(game, 1);
	clientes = new Pool<Cliente>(game, 50);
	grupoClientes = new Pool<GrupoClientes>(game, 20);
}

ObjectManager::~ObjectManager()
{
	for (auto i : muebles) {
		delete i;
		i = nullptr;
	}

	for (auto i : players) {
		delete i;
		i = nullptr;
	}

	delete ingredientes;
}

void ObjectManager::render(SDL_Rect* rect)
{
	for (auto m : muebles)
		m->render(rect);

	for (auto i : paellas)
		i->render(rect);

	for (auto p : players)
		p->render(rect);

	if(arroz_!=nullptr)arroz_->render(rect);
	

	/*for (auto c : comandas)
		c->render(rect);

	for (auto i : interfaz)
		i->render(rect);*/

	ingredientes->render(rect);

	grupoClientes->render(rect);


}

void ObjectManager::debug(SDL_Rect* rect)
{
	for (auto i : muebles)
		i->renderDebug(rect);

	for (auto i : players)
		i->renderDebug(rect);

	for (auto i : paellas)
		i->renderDebug(rect);



	ingredientes->debug(rect);

	clientes->debug(rect);
}

void ObjectManager::handleInput(bool& exit)
{
	// solo se handlea tu propio input
	Player* p = getHost();
	if (p != nullptr)
		p->handleInput();
}

void ObjectManager::update()
{
	for (auto i : muebles)
		i->update();

	for (auto i : players)
		i->update();

	for (auto i : paellas)
		i->update();

	ingredientes->update();	

	grupoClientes->update();

	SDL_GetMouseState(&x, &y);

	SDL_Rect rect = SDL_Rect{ x, y, range, range };

	for (auto i : grupoClientes->getCollisions(rect)) {
		i->ratonEncima();
	}
}

void ObjectManager::addMueble(GameObject* mueble)
{
	muebles.push_back(mueble);
}

void ObjectManager::addPlayer(Player* player)
{
	players.push_back(player);
}

void ObjectManager::addComanda(GameObject* comanda)
{
	interfaz.push_back(comanda);
}

void ObjectManager::addPaella(GameObject* paella)
{
	paellas.push_back(paella);
}

void ObjectManager::addArroz(Arroz* arroz) {
	arroz_ = arroz;
}

void ObjectManager::arrozColocado()
{
	arroz_ = nullptr;
}


vector<Collider*> ObjectManager::getMueblesCollider()
{
	vector<Collider*> c;

	for (auto i : muebles)
		c.push_back(i);

	return c;
}

vector<Collider*> ObjectManager::getMueblesCollider(SDL_Rect collider)
{
	vector<Collider*> c;

	for (auto i : muebles) {
		if (i->collide(collider))
			c.push_back(i);
	}

	return c;
}

vector<Collider*> ObjectManager::getPaellasCollider(SDL_Rect collider)
{
	vector<Collider*> c;

	for (auto i : paellas) {
		if (i->collide(collider))
			c.push_back(i);
	}

	return c;
}



void ObjectManager::initMuebles()
{
	for (auto i : muebles)
		i->init(this);
}

