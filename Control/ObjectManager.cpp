#include "ObjectManager.h"
#include "../../GameObjects/Muebles/InicioCinta.h"
#include "../../GameObjects/Muebles/FinalCinta.h"
#include "../../GameObjects/Muebles/Puerta.h"
#include  "../RedactaComandabutton.h"
#include "../Data/Comanda.h"
#include "../GameObjects/Muebles/cartel.h"
#include "../../Utils/Vector2D.h"


ObjectManager::ObjectManager(Game* game)
{
	ingredientes = new Pool<Ingrediente>(game, 3);
	clientes = new Pool<Cliente>(game, 30);
	grupoClientes = new Pool<GrupoClientes>(game, 20);

	muebles.push_back(new InicioCinta(game, ingredientes));
	muebles.push_back(new FinalCinta(game, ingredientes));
	muebles.push_back(new Puerta(game, clientes, grupoClientes));
	muebles.push_back(new Cartel(game));
	interfaz.push_back(new RedactaComandabutton(game, botonredacta,10,10,30,30));
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
	for (auto i : muebles)
		i->draw();
	for (auto z : comandas)
	{
		z->draw();
	}
	for (auto j : interfaz)
	{ 
		j->draw();
	}

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

void ObjectManager::update()
{
	for (auto i : muebles)
		i->update();

	ingredientes->update();	

	clientes->update();
}

pair<TextureName, int> ObjectManager::getRandomIngridient()
{
	int n = rand() % (texturasIngredienes.size());

	return { texturasIngredienes[n], n};
}
void ObjectManager::Uievent(int mx, int my)
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

