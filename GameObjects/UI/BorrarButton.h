#pragma once
#include "UiButton.h"
class Game;
class BorrarButton: public UiButton
{
	Game* gamet;

public:
	BorrarButton(Game* game, string texturename, int x, int y, int w, int h);
	~BorrarButton();
	void execute()override;
};

