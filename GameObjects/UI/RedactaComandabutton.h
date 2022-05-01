#pragma once
#include "UiButton.h"
#include "../../Data/Comanda.h"

class Game;
class UIManager;

class RedactaComandabutton : public UiButton
{
	UIManager* uim;
	
public:
	RedactaComandabutton(Game* game, UIManager* uiManager, string claveTextura, int x, int y, int w, int h);
	~RedactaComandabutton();
	void Swichposition();
	void execute(bool& exit) override;
};

