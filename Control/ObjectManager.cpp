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
	ingredientes = new Pool<Ingrediente>(game, 10);
	clientes = new Pool<Cliente>(game, 30);
	grupoClientes = new Pool<GrupoClientes>(game, 20);

	muebles.push_back(new InicioCinta(game, ingredientes));
	muebles.push_back(new FinalCinta(game, ingredientes));
	muebles.push_back(new Puerta(game, clientes, grupoClientes));
	muebles.push_back(new Cartel(game));
}

ObjectManager::~ObjectManager()
{
	for (auto i : muebles) {
		delete i;
		i = nullptr;
	}

	delete ingredientes;
}

void ObjectManager::render()
{
	for (auto m : muebles)
		m->render();

	/*for (auto c : comandas)
		c->render();

	for (auto i : interfaz)
		i->render();*/

	ingredientes->render();

	clientes->render();
}

void ObjectManager::debug()
{
	for (auto i : muebles)
		i->drawDebug();

	ingredientes->debug();

	clientes->debug();
}

void ObjectManager::handleInput()
{
}

void ObjectManager::update()
{
	for (auto i : muebles)
		i->update();

	ingredientes->update();	

	grupoClientes->update();

	SDL_GetMouseState(&x, &y);

	SDL_Rect rect = SDL_Rect{ x, y, range, range };


	for (auto i : grupoClientes->getCollisions(rect)) {
		i->ratonEncima();
	}
}


vector<Collider*> ObjectManager::getClientes(SDL_Rect gOC)
{
	return clientes->getCollisions(gOC);
}

vector<Collider*> ObjectManager::getIngredientes(SDL_Rect gOC)
{
	return ingredientes->getCollisions(gOC);
}

