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
	ingredientes = new Pool<Ingrediente>(game, 50);
	clientes = new Pool<Cliente>(game, 50);
	grupoClientes = new Pool<GrupoClientes>(game, 20);
	arroces = new Pool<Arroz>(game, 20);
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
	sortAllGO();

	for (auto i : renderAll)
		i->render(rect);

	for (auto i : paellas) {
		i->render(rect);
	}
		
	
	ingredientes->render(rect);

	arroces->render(rect);

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
	for (auto i : renderAll) 
		i->update();

	cout << paellas.size() << endl;

	for (auto i : paellas)
		i->update();

	for (auto i : muebles)
		i->update();

	ingredientes->update();	

	arroces->update();	

	grupoClientes->update();

	SDL_GetMouseState(&x, &y);

	SDL_Rect rect = SDL_Rect{ x, y, range, range };

	for (auto i : grupoClientes->getCollisions(rect)) {
		dynamic_cast<GrupoClientes*>(i)->ratonEncima();
	}
}

void ObjectManager::addMueble(GameObject* mueble)
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

