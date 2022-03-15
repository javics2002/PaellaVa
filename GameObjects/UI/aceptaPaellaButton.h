#pragma once
#include "UiButton.h"
class Comanda;
class Game;
class aceptaPaellaButton:public UiButton
{
	Comanda* comandat;
	Game* gamet;
public:
	aceptaPaellaButton(Game* game,Comanda* c, string texturename, int x, int y, int w, int h);
	~aceptaPaellaButton();
	void execute()override;
};

