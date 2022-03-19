#include "Scene.h"

Scene::Scene(Game* game) 
{
	this->game = game;

	objectManager = new ObjectManager(game);
	uiManager = new UIManager(game);
	fondo = new Fondo(game);
	camara = new Camera(*new Vector2D<float>(0, 16), sdlutils().width(), sdlutils().height());
}

Scene::~Scene()
{
	delete uiManager;
	delete objectManager;
	delete fondo;
	delete camara;

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
	fondo->render(camara->renderRect());
	objectManager->render(camara->renderRect());
	uiManager->render(nullptr); // ponemos nullptr para que se mantenga en la pantalla siempre
}

void Scene::debug()
{
	fondo->renderDebug(camara->renderRect());
	objectManager->debug(camara->renderRect());
	//objectManager->debug();
}

ObjectManager* Scene::getObjectManager()
{
	return objectManager;
}

UIManager* Scene::getUIManager()
{
	return uiManager;
}