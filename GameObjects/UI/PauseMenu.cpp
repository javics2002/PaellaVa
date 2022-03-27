#include "PauseMenu.h"

PauseMenu::PauseMenu(Game* game) : GameObject(game)
{
	this->game = game;
	setDimension(w, h);
	setPosition(sdlutils().width() / 2, sdlutils().height() / 2);
	// ni idea de lo de abajo
	setActive(false);

	setTexture("pause1");
}

PauseMenu::~PauseMenu()
{
}

void PauseMenu::render(SDL_Rect* cameraRect)
{
	drawRender(cameraRect);
}


void PauseMenu::update()
{

}
