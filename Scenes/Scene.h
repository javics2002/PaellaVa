#pragma once

#pragma once

#include "../Control/ObjectManager.h"
#include "../GameObjects/UI/UIManager.h"
#include "../GameObjects/Fondo.h"
#include "../sdlutils/SDLUtils.h"
#include "../Control/Game.h"

class Scene {

protected:
	Game* game;
	ObjectManager* objectManager;

	UIManager* uiManager;

	Fondo* fondo;

public:
	Scene(Game* game);
	virtual ~Scene();

	virtual void handleInput();
	virtual void update();
	virtual void render();
	virtual void debug();

	ObjectManager* getObjectManager();
	UIManager* getUIManager();

};