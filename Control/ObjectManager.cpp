#include "ObjectManager.h"
#include "../../GameObjects/Muebles/InicioCinta.h"
#include "../../GameObjects/Muebles/FinalCinta.h"
#include "../../Utils/Vector2D.h"

ObjectManager::ObjectManager(Game* game)
{
	ingredientes = new Pool<Ingrediente>(game, 3);

	muebles.push_back(new InicioCinta(game, ingredientes));
	muebles.push_back(new FinalCinta(game, ingredientes));
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

	ingredientes->render();	
}

void ObjectManager::debug()
{
	for (auto i : muebles)
		i->drawDebug();

	ingredientes->debug();
}

void ObjectManager::update()
{
	for (auto i : muebles)
		i->update();

	ingredientes->update();	
}



