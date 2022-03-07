#pragma once

#include "../Control/ObjectManager.h"
#include "../Scenes/Scene.h"
#include "SDL.h"

using namespace std;

class Game;

class Menu : public Scene
{

	Game* game;

	ObjectManager* objectManager;

public:
	Menu(Game* game);
	~Menu();

	void handleInput();
	void update();
	void render();

	ObjectManager* getObjectManager();


};