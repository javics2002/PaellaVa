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

	//interfaz.push_back(new RedactaComandabutton(game, "redactaboton", 10, 10, 30, 30));
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

void ObjectManager::render(SDL_Rect& rect)
{
	for (auto m : muebles)
		m->render(rect);

	/*for (auto c : comandas)
		c->render(rect);

	for (auto i : interfaz)
		i->render(rect);*/

	ingredientes->render(rect);

	clientes->render(rect);
}

void ObjectManager::debug()
{
	for (auto i : muebles)
		i->drawDebug();

	ingredientes->debug();

	clientes->debug();
}

void ObjectManager::debug(SDL_Rect& rect)
{
	for (auto i : muebles)
		i->drawDebug(rect);

	ingredientes->debug(rect);

	clientes->debug(rect);
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

void ObjectManager::addMueble(GameObject* mueble)
{
	muebles.push_back(mueble);
}

void ObjectManager::addComanda(GameObject* comanda)
{
	interfaz.push_back(comanda);
}



vector<Collider*> ObjectManager::getClientes(SDL_Rect gOC)
{
	return clientes->getCollisions(gOC);
}

vector<Collider*> ObjectManager::getIngredientes(SDL_Rect gOC)
{
	return ingredientes->getCollisions(gOC);
}

vector<Collider*> ObjectManager::getGrupoClientes(SDL_Rect gOC)
{
	return grupoClientes->getCollisions(gOC);
}

