#pragma once
#include "UiButton.h"
class Game;
class Comanda;
class DescartaCommandaButton:public UiButton
{
	Game* gamet;
	Comanda* comandat;
public:
	DescartaCommandaButton(Comanda* c,Game* game, string texturename, int x, int y, int w, int h);
	~DescartaCommandaButton();
	void execute()override;
};

