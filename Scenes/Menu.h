#pragma once

#include "../Control/ObjectManager.h"
#include "../Scenes/Scene.h"

class Menu : public Scene
{
public:
	Menu(Game* mGame);
	void handleInput(bool& exit) override;
};