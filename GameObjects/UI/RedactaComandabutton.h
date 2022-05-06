#pragma once
#include "UiButton.h"
#include "../../Data/Comanda.h"

class Game;
class UIManager;

class RedactaComandabutton : public UiButton
{
	UIManager* uim;
	
public:
	RedactaComandabutton(Game* mGame, UIManager* uiManager, string claveTextura, int x, int y, int w, int h);
	~RedactaComandabutton();
	void Swichposition();
	void goDown();
	void resetPosition();
	void seteabutton();
	void execute(bool& exit) override;
};

