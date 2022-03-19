#pragma once
#include "UiButton.h"
class Game;
class Comanda;
class BorrarButton: public UiButton
{
	Comanda* comanda;

public:
	BorrarButton(Game* game, Comanda* c ,string texturename, int x, int y, int w, int h);
	~BorrarButton();
	void execute()override;
};

