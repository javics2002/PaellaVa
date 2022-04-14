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


enum States {
	cogerClientes,pausaClientes, dejarClientesMesa,pausaPaellas,apuntaPedido,pausaPedido, cogePaellera,pausaCogePaellera, dejaPaellera,pausaDejaPaella,cogerDejarArroz, cogerIngrediente,
	procesarIngrediente, dejarIngredientePaella, cocinarPaella, darDeComerPaella
};



class Scene {

protected:
	Game* game;
	ObjectManager* objectManager;
	Camera* camara;

	States currentState = cogerClientes;
	UIManager* uiManager;

	Imagen* fondo;
	bool paused = false;

public:
	Scene(Game* game);
	virtual ~Scene();

	virtual void handleInput(bool& exit);
	virtual void update();
	virtual void refresh() {};
	virtual void pauseTutorial() {};
	virtual void render();
	virtual void debug();
	virtual void addPuntuaciones(double puntosComanda) {};
	virtual void changeState(States state) {};
	virtual States getState() { return currentState; };


	ObjectManager* getObjectManager();
	UIManager* getUIManager();
	virtual Player* getPlayer() { return nullptr; };
};