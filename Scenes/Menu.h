#pragma once

#include "../Control/ObjectManager.h"
#include "../Scenes/Scene.h"
#include "SDL.h"

using namespace std;

class Game;

class Menu : public Scene
{

public:
	Menu(Game* game);
	~Menu();

	void handleInput() override;
	void update() override;
	void render() override;
	void debug() override;

};