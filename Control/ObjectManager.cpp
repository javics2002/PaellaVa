#include "ObjectManager.h"
#include  "../GameObjects/UI/RedactaComandabutton.h"
#include "../Data/Comanda.h"
#include "../GameObjects/Muebles/cartel.h"
#include "../Utils/Vector2D.h"


ObjectManager::ObjectManager(Game* game) : game(game)
{
	pools.emplace_back((Pool<GameObject>*) new Pool<Ingredienteletal>(game, 10));
	pools.emplace_back((Pool<GameObject>*) new Pool<Ingrediente>(game, 50));
	
	pools.emplace_back((Pool<GameObject>*) new Pool<Arroz>(game, 20));
	pools.emplace_back((Pool<GameObject>*) new Pool<GrupoClientes>(game, 20));
	pools.emplace_back((Pool<GameObject>*) new Pool<Cliente>(game, 50));
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
		i = nullptr;
	}
}

void ObjectManager::render(SDL_Rect* rect)
{
	vector<GameObject*> aux = renderAll;
	
	for (auto i : pools) {
		for (auto o : i->getActiveObjects()) {
			renderAll.emplace_back(o);
		}
	}

	sortAllGO();

	for (auto i : renderAll) {
		i->render(rect);
	}

	renderAll = aux;	
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
	for (auto i : players)
		i->update();

	for (auto i : muebles)
		i->update();

	for (auto i : paellas)
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

	renderAll.push_back(p);

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
		return a->getDepth() < b->getDepth() || (a->getDepth() == b->getDepth() && a->getY() < b->getY());
		});
}

void ObjectManager::initMuebles()
{
	for (auto i : muebles)
		i->init(this);
}

void ObjectManager::addSilla(Silla* s)
{
	sillas.push_back(s);
}

void ObjectManager::addMesa(Mesa* m)
{
	mesas.push_back(m);
}

void ObjectManager::addVentanilla(Ventanilla* v)
{
	ventanillas.push_back(v);
}

void ObjectManager::addEncimera(Encimera* e)
{
	encimeras.push_back(e);
}


void ObjectManager::addCinta(Cinta* c)
{
	cintas.push_back(c);
}

void ObjectManager::addCartel(Cartel* c)
{
	cartel = c;
}

void ObjectManager::addPuerta(Puerta* p)
{
	puerta = p;
}

vector<Silla*> ObjectManager::getSillas()
{
	return sillas;
}

vector<Mesa*> ObjectManager::getMesas()
{
	return mesas;
}

vector<Encimera*> ObjectManager::getEncimeras()
{
	return encimeras;
}

vector<Ventanilla*> ObjectManager::getVentanilla()
{
	return ventanillas;
}



vector<Cinta*> ObjectManager::getCintas()
{
	return cintas;
}

Cartel* ObjectManager::getCartel()
{
	return cartel;
}

Puerta* ObjectManager::getPuerta()
{
	return puerta;
}
