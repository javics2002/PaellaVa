#include "ObjectManager.h"
#include  "../GameObjects/UI/RedactaComandabutton.h"
#include "../Data/Comanda.h"
#include "../GameObjects/Muebles/MueblesInclude.h"
#include "../Utils/Vector2D.h"
#include "../Scenes/Tutorial.h"
#include "../GameObjects/Herramienta.h"
#include "../GameObjects/CajaTakeaway.h"


ObjectManager::ObjectManager(Game* mGame) : mGame(mGame)
{
	if (dynamic_cast<Tutorial*>(mGame->getCurrentScene())) {
		pools.emplace_back((Pool<GameObject>*) new Pool<Ingrediente>(mGame, 50));
	}
	else {
		pools.emplace_back((Pool<GameObject>*) new Pool<IngredienteLetal>(mGame, 10));
		pools.emplace_back((Pool<GameObject>*) new Pool<Ingrediente>(mGame, 50));
	}
	pools.emplace_back((Pool<GameObject>*) new Pool<Herramienta>(mGame, 20));
	pools.emplace_back((Pool<GameObject>*) new Pool<CajaTakeaway>(mGame, 20));
	pools.emplace_back((Pool<GameObject>*) new Pool<Arroz>(mGame, 20));
	pools.emplace_back((Pool<GameObject>*) new Pool<Repartidor>(mGame, 30));
	pools.emplace_back((Pool<GameObject>*) new Pool<GrupoClientes>(mGame, 20));
	pools.emplace_back((Pool<GameObject>*) new Pool<Cliente>(mGame, 50));
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
	Player* p = getPlayerOne();
	if (p != nullptr)
		p->handleInput(ih().getAxis(), true); // Handle input del primer jugador

	Player* p2 = getPlayerTwo();
	if (p2 != nullptr)
		p2->handleInput(ih().getOtherAxis(), false); // Handle input del segundo jugador
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
	Paella* p = new Paella(mGame, n);

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

	// get Repart pool

	return c;
}

void ObjectManager::sortAllGO() {
	std::sort(renderAll.begin(), renderAll.end(), [](GameObject* a, GameObject* b) {
		return a->getDepth() < b->getDepth() || 
			(a->getDepth() == b->getDepth() && a->getPosVertical() < b->getPosVertical());
		});
}

void ObjectManager::resetMueblesCounter()
{
	for (auto i : muebles)
	{
		i->resetCounter();
	}
}

void ObjectManager::initMuebles()
{
	for (auto i : muebles)
		i->init(this);
}


