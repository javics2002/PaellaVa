#pragma once
#include "UiButton.h"
#include "../../Data/Comanda.h"

class NumeroButton : public UiButton
{
	string textura;
	Comanda* c;
public: 
	NumeroButton(UIManager* uim, Game* game, string texturename, int x, int y, int w, int h);
	~NumeroButton();
	void execute(bool& exit) override;
	void update() override;
};