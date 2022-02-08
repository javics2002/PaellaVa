#include "ObjectManager.h"

ObjectManager::ObjectManager(Game* game)
{
	ingredientes = new Pool<Ingrediente>(game, 6);
}

ObjectManager::~ObjectManager()
{
	delete ingredientes;
}

