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

ObjectManager::ObjectManager(Game* game) : game(game)
{
	auto ingredinetes = new Pool<Ingrediente>(game, 50);
	auto arrozes = new Pool<Arroz>(game, 20);
	auto grupos = new Pool<GrupoClientes>(game, 20);
	auto clientes = new Pool<Cliente>(game, 50);


	pools.emplace_back((Pool<GameObject>*) ingredinetes);
	pools.emplace_back((Pool<GameObject>*) arrozes);
	pools.emplace_back((Pool<GameObject>*) grupos);
	pools.emplace_back((Pool<GameObject>*) clientes);
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

	for (auto i : pools) {
		delete i;
	}
}

void ObjectManager::render(SDL_Rect* rect)
{
	sortAllGO();

	for (auto i : renderAll)
		i->render(rect);

	for (auto i : paellas) {
		i->render(rect);
	}
		
	for (auto i : pools) {
		i->render(rect);
	}
}

void ObjectManager::debug(SDL_Rect* rect)
{
	for (auto i : muebles)
		i->renderDebug(rect);

	for (auto i : players)
		i->renderDebug(rect);

	for (auto i : paellas)
		i->renderDebug(rect);

	for (auto i : pools)
		i->debug(rect);
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
	for (auto i : renderAll) 
		i->update();

	for (auto i : paellas)
		i->update();

	for (auto i : muebles)
		i->update();

	for (int i = 0; i < pools.size() - 1; i++)
		pools[i]->update();
}

void ObjectManager::refresh()
{
	for (auto i : pools)
		i->refresh();
}

void ObjectManager::addMueble(Mueble* mueble)
{
	muebles.push_back(mueble);
	
	renderAll.push_back(mueble);
}

void ObjectManager::addPlayer(Player* player)
{
	players.push_back(player);

	renderAll.push_back(player);
}

Paella* ObjectManager::addPaella(int n)
{
	Paella* p = new Paella(game, n);

	paellas.push_back(p);

	return p;
}

vector<Mueble*> ObjectManager::getMueblesCollisions(SDL_Rect collider)
{
	vector<Mueble*> c;

	for (auto i : muebles) {
		if (i->collide(collider))
			c.push_back(i);
	}

	return c;
}

vector<Mueble*> ObjectManager::getMueblesOverlaps(SDL_Rect collider)
{
	vector<Mueble*> c;

	for (auto i : muebles) {
		if (i->overlap(collider))
			c.push_back(i);
	}

	return c;
}

void ObjectManager::sortAllGO() {
	std::sort(renderAll.begin(), renderAll.end(), [](GameObject* a, GameObject* b) {
		return a->getY() < b->getY();
		});
}

void ObjectManager::initMuebles()
{
	for (auto i : muebles)
		i->init(this);
}

