#include "Scene.h"

Scene::Scene(Game* game) 
{
	this->game = game;

	objectManager = new ObjectManager(game);
	uiManager = new UIManager(game);
	textMngr = new TextManager(game, "abadiT");
	fondo = new Imagen(game);
	fondo->setTexture("menufondo");
	camara = new Camera(*new Vector2D<float>(0, 16), sdlutils().width(), sdlutils().height());
}

Scene::~Scene()
{
	delete uiManager;
	delete objectManager;
	delete fondo;
	delete camara;
}

void Scene::handleInput(bool& exit)
{
	if(!paused)objectManager->handleInput(exit);
	uiManager->handleInput(exit, paused);

	if (ih().getKey(InputHandler::INTERACT))
		uiManager->focoExecute(exit);

	if (ih().getKey(InputHandler::RIGHT) || ih().getKey(InputHandler::DOWN))
		uiManager->avanzaFoco();
	if (ih().getKey(InputHandler::LEFT) || ih().getKey(InputHandler::UP))
		uiManager->retrocedeFoco();
}

void Scene::update()
{
	objectManager->update();
	uiManager->update(paused);
	textMngr->update();
}

void Scene::render()
{
	fondo->render(camara->renderRect());
	objectManager->render(camara->renderRect());
	uiManager->render(nullptr); // ponemos nullptr para que se mantenga en la pantalla siempre
	textMngr->render();
}


void Scene::debug()
{
	fondo->renderDebug(camara->renderRect());
	objectManager->debug(camara->renderRect());
}

ObjectManager* Scene::getObjectManager()
{
	return objectManager;
}

UIManager* Scene::getUIManager()
{
	return uiManager;
}