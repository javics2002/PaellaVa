#pragma once
#include "UiButton.h"

class Comanda;
class Game;

class AceptaPaellaButton:public UiButton
{
	Comanda* comanda;

public:
	AceptaPaellaButton(Game* game,Comanda* c, string texturename, int x, int y, int w, int h);
	~AceptaPaellaButton();
	void execute(bool& exit)override;
};
