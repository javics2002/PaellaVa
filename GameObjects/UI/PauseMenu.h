#pragma once
#include "../GameObject.h"

class PauseMenu : public GameObject
{
	// Menu de pausa
public:
	PauseMenu(Game* game);
	~PauseMenu();

	void render(SDL_Rect* cameraRect) override;
	void update() override;

private:
	int h = 300;
	int w = 300;
};

