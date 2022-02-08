#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
	ingredientes = new Pool<Ingrediente>(6);
}

ObjectManager::~ObjectManager()
{
	delete ingredientes;
}

