#include "Scene.h"

Scene::Scene(Game* game) 
{
	this->game = game;

	objectManager = new ObjectManager(game);
	uiManager = new UIManager(game);
	fondo = new Fondo(game);
}

Scene::~Scene()
{
	delete uiManager;
	delete objectManager;
	delete fondo;
}

void Scene::handleInput()
{
	objectManager->handleInput();
	//uiManager->handleInput();

}

void Scene::update()
{
	objectManager->update();
	uiManager->update();
}

void Scene::render()
{
	objectManager->render();
	uiManager->render();
	fondo->render();
}

void Scene::debug()
{
	fondo->drawDebug();
	//uiManager->drawDebug();
	objectManager->debug();
}

ObjectManager* Scene::getObjectManager()
{
	return objectManager;
}

UIManager* Scene::getUIManager()
{
	return uiManager;
}
