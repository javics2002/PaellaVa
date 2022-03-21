#pragma once
#include "UiButton.h"
#include "../../Data/Comanda.h"
class Tamanobutton: public UiButton
{
	Comanda* c;
	string textura;
public:
	Tamanobutton(UIManager* uim, Game* game, string texturename, int x, int y, int w, int h);
	~Tamanobutton();
	void execute() override;
	void update() override;
};

