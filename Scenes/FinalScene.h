#pragma once

#include "../Scenes/Scene.h"

class FinalScene : public Scene
{
public:
	FinalScene(Game* mGame);

private:
	void render() override;
};

