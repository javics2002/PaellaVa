#pragma once
#include "UiButton.h"
class Game;
class Comanda;
class BorrarButton: public UiButton
{
	Comanda* comanda;

public:
	BorrarButton(Game* mGame, Comanda* c ,string texturename, int x, int y, int w, int h);
	~BorrarButton();
	void execute(bool& exit)override;
};

