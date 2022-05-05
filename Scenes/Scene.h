#pragma once

#pragma once

#include "../Control/ObjectManager.h"
#include "../GameObjects/UI/UIManager.h"
#include "../GameObjects/Imagen.h"
#include "../sdlutils/SDLUtils.h"
#include "../Control/Game.h"
#include "../Control/TextManager.h"
#include "../Control/Camera.h"
#include "SDL.h"

class Player;

// Estados para el tutorial
enum States { cogerClientes, pausaClientes, dejarClientesMesa,
	pausaPaellas, abreLibreta, pauasAbreLibreta,
	apuntaPedido ,pausaPedido, cogePaellera,pausaCogePaellera, 
	dejaPaellera,pausaDejaPaella,cogerDejarArroz,
	pausaCogerDejarArroz, arreglarCinta, pausaArreglarCinta,
	cogerIngrediente, pausaCogerIngrediente, procesarIngrediente, 
	pausaProcesarIngrediente, dejarIngredientePaella, pausaDejarIngredientes, 
	cocinarPaella, pausaCocinarPaella, recogerPaellaCocinada, 
	pausaRecogerPaellaCocinada, darDeComer, pausaDarDeComer, 
	sacarCuenta, pausaSacarCuenta, echarClientes,
	pausaEcharClientes, recogerMesa, pausaRecogerMesa,
	dejarLavavajillas, pausaDejarLavavajillas, cogerLavavajillas,
	pausaCogerLavavajillas, dejarPaellaPila, pausaFinal,final, 
	pausaComandaEquivocada, pausaBorrarComanda, pausaNoEcharClientes, pausaInicio
};



class Scene {

protected:
	Game* mGame;
	ObjectManager* mObjectManager;
	Camera* mCamera;

	States mCurrentState = cogerClientes, anteriorEstado;
	UIManager* mUiManager;

	Imagen* mBackground;
	bool mPaused = false;
	TextManager* mTextMngr;

public:
	Scene(Game* mGame);
	virtual ~Scene();

	virtual void handleInput(bool& exit);
	virtual void update();
	virtual void refresh() {};
	virtual void pauseTutorial() {};
	virtual void render();
	virtual void debug();
	virtual void addPuntuaciones(double puntosComanda) {};
	virtual void changeState(States state) {};
	virtual States getState() { return mCurrentState; };


	ObjectManager* getObjectManager();
	UIManager* getUIManager();
	virtual Player* getPlayer() { return nullptr; };
};