#include "Scene.h"

Scene::Scene(Game* mGame) 
{
	this->mGame = mGame;

	mObjectManager = new ObjectManager(mGame);
	mUiManager = new UIManager(mGame);
	mTextMngr = new TextManager(mGame, "abadiT");
	mBackground = new Imagen(mGame);
	mBackground->setTexture("menufondo");
	mCamera = new Camera(*new Vector2D<float>(0, 16), sdlutils().width(), sdlutils().height());
}

Scene::~Scene()
{
	delete mUiManager;
	delete mObjectManager;
	delete mTextMngr;
	delete mBackground;
	delete mCamera;

}

void Scene::handleInput(bool& exit)
{
	if(!mPaused)mObjectManager->handleInput(exit);
	mUiManager->handleInput(exit, mPaused);

	if (ih().getKey(InputHandler::A))
		mUiManager->focoExecute(exit);

	if (ih().getKey(InputHandler::RIGHT) || ih().getKey(InputHandler::DOWN))
		mUiManager->avanzaFoco();
	if (ih().getKey(InputHandler::LEFT) || ih().getKey(InputHandler::UP))
		mUiManager->retrocedeFoco();
}

void Scene::update()
{
	mObjectManager->update();
	mUiManager->update(mPaused);
	mTextMngr->update();
}

void Scene::render()
{
	mBackground->render(mCamera->renderRect());
	mObjectManager->render(mCamera->renderRect());
	mUiManager->render(nullptr); // ponemos nullptr para que se mantenga en la pantalla siempre
	mTextMngr->render();
}


void Scene::debug()
{
	mBackground->renderDebug(mCamera->renderRect());
	mObjectManager->debug(mCamera->renderRect());
}

ObjectManager* Scene::getObjectManager()
{
	return mObjectManager;
}

UIManager* Scene::getUIManager()
{
	return mUiManager;
}