#pragma once
#include "UiButton.h"
#include "Data/Comanda.h"
class Game;
class RedactaComandabutton:public UiButton
{
	Game* gamet;
	
public:
	RedactaComandabutton(Game* game, TextureName texturename, int x, int y, int w, int h);
	~RedactaComandabutton();
	void execute()override;
};

