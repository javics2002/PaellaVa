#pragma once
#include "UiButton.h"
#include "../../Data/Comanda.h"
class Game;
class UIManager;
class RedactaComandabutton:public UiButton
{
	Game* gamet;
	UIManager* uim;

	
public:
	RedactaComandabutton(Game* game, string claveTextura, int x, int y, int w, int h);
	~RedactaComandabutton();
	void execute()override;
};

