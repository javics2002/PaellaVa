#pragma once

#pragma once

#include "../Control/ObjectManager.h"
#include "../GameObjects/UI/UIManager.h"
#include "../GameObjects/Imagen.h"
#include "../sdlutils/SDLUtils.h"
#include "../Control/Game.h"
#include "../Control/Camera.h"
#include "SDL.h"

class Player;

class Scene {

protected:
	Game* game;
	ObjectManager* objectManager;
	Camera* camara;

	UIManager* uiManager;

	Imagen* fondo;
	bool paused = false;

public:
	Scene(Game* game);
	virtual ~Scene();

	virtual void handleInput(bool& exit);
	virtual void update();
	virtual void refresh() {};
	virtual void render();
	virtual void debug();
	virtual void addPuntuaciones(double puntosComanda) {};

	ObjectManager* getObjectManager();
	UIManager* getUIManager();
	virtual Player* getPlayer() { return nullptr; };
};