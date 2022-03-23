#pragma once
#include "UiButton.h"
#include "../../Data/Comanda.h"

class TamanoButton: public UiButton
{
	Comanda* c;
	string textura;
public:
	TamanoButton(UIManager* uim, Game* game, string texturename, int x, int y, int w, int h);
	~TamanoButton();
	void execute(bool& exit) override;
	void update() override;
};

