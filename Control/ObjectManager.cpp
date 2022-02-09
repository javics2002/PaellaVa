#include "ObjectManager.h"
#include "../../GameObjects/Muebles/InicioCinta.h"
#include "../../Utils/Vector2D.h"

ObjectManager::ObjectManager(Game* game)
{
	ingredientes = new Pool<Ingrediente>(game, 5);

	muebles.push_back(new InicioCinta(game, ingredientes));
}

ObjectManager::~ObjectManager()
{
	delete ingredientes;
}

void ObjectManager::render()
{
	ingredientes->render();

	for (auto i : muebles)
		i->draw();
}

void ObjectManager::update()
{
	ingredientes->update();

	for (auto i : muebles)
		i->update();
}

