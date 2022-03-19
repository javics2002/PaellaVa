#pragma once

#include "../Control/ObjectManager.h"
#include "../Scenes/Scene.h"

class GameOver : public Scene
{
public:
	GameOver(Game* game);
	~GameOver();

	void render() override;
	void debug() override;
};

