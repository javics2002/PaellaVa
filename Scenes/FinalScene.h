#pragma once

#include "../Scenes/Scene.h"

class FinalScene : public Scene
{
public:
	FinalScene(Game* game, int mediaPuntuacion);

private:
	void render() override;
};

