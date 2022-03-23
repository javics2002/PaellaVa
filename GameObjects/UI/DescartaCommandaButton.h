#pragma once
#include "UiButton.h"

class Game;
class Comanda;

class DescartaCommandaButton:public UiButton
{
	Comanda* comanda;
public:
	DescartaCommandaButton(Comanda* c,Game* game, string texturename, int x, int y, int w, int h);
	~DescartaCommandaButton();
	void execute(bool& exit)override;
};

