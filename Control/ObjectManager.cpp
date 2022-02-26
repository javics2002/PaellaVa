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
	ingredientes = new Pool<Ingrediente>(game, 3);
	clientes = new Pool<Cliente>(game, 30);
	grupoClientes = new Pool<GrupoClientes>(game, 20);

	muebles.push_back(new InicioCinta(game, ingredientes));
	muebles.push_back(new FinalCinta(game, ingredientes));
	muebles.push_back(new Puerta(game, clientes, grupoClientes));
	muebles.push_back(new Cartel(game));
	interfaz.push_back(new RedactaComandabutton(game, "redactaboton", 10, 10, 30, 30));
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

	for (auto c : comandas)
		c->render();

	for (auto i : interfaz)
		i->render();

	ingredientes->render();	

	clientes->render();
}

void ObjectManager::handleInput()
{
}

void ObjectManager::update()
{
	for (auto i : muebles)
		i->update();

	ingredientes->update();	

	clientes->update();
}

void ObjectManager::uiEvent(int mx, int my)
{
	for (auto i : interfaz)
	{
		i->OnClick(mx, my);
	}
}
void ObjectManager::creaComanda(Game*game)
{
	comandas.push_back(new Comanda(game, 2));
}

vector<Collider*> ObjectManager::getClientes(SDL_Rect gOC)
{

	return clientes->getCollisions(gOC);
}


