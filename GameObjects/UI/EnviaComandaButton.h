#pragma once
#include "UiButton.h"
class Game;
class EnviaComandaButton : public UiButton
{
public:
	EnviaComandaButton(Game* game, string texturename, int x, int y, int w, int h);
	~EnviaComandaButton();
	void execute()override;
};

