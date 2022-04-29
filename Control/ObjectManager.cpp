#include "ObjectManager.h"
#include  "../GameObjects/UI/RedactaComandabutton.h"
#include "../Data/Comanda.h"
#include "../GameObjects/Muebles/MueblesInclude.h"
#include "../Utils/Vector2D.h"
#include "../Scenes/Tutorial.h"
#include "../GameObjects/Herramienta.h"


ObjectManager::ObjectManager(Game* game) : game(game)
{
	if (dynamic_cast<Tutorial*>(game->getCurrentScene())) {
		pools.emplace_back((Pool<GameObject>*) new Pool<Ingrediente>(game, 50));
	}
	else {
		pools.emplace_back((Pool<GameObject>*) new Pool<IngredienteLetal>(game, 10));
		pools.emplace_back((Pool<GameObject>*) new Pool<Ingrediente>(game, 50));
	}
	pools.emplace_back((Pool<GameObject>*) new Pool<Herramienta>(game, 20));
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

void ObjectManager::addIniCinta(InicioCinta* i)
{
	iniCinta = i;
}

void ObjectManager::addFinCinta(FinalCinta* f)
{
	finCinta = f;
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

void ObjectManager::addPilas(Pila* p)
{
	pilas.push_back(p);
}

void ObjectManager::addBolsa(BolsaArroz* b)
{
	bolsaDeArroz = b;
}

void ObjectManager::addFogon(Fogon* f)
{
	fogones.push_back(f);
}

void ObjectManager::addTabla(TablaProcesado* t)
{
	tablasProcesados.push_back(t);
}

void ObjectManager::addLavavajillas(Lavavajillas* l)
{
	lavavajillas = l;
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

vector<Pila*> ObjectManager::getPilas()
{
	return pilas;
}

vector<Fogon*> ObjectManager::getFogones()
{
	return fogones;
}

vector<TablaProcesado*> ObjectManager::getTablas()
{
	return tablasProcesados;
}

Cartel* ObjectManager::getCartel()
{
	return cartel;
}

Puerta* ObjectManager::getPuerta()
{
	return puerta;
}

BolsaArroz* ObjectManager::getBolsa()
{
	return bolsaDeArroz;
}

InicioCinta* ObjectManager::getIniCinta()
{
	return iniCinta;
}

FinalCinta* ObjectManager::getFinCinta()
{
	return finCinta;
}

Lavavajillas* ObjectManager::getLavavajillas()
{
	return lavavajillas;
}
