#pragma once
#include "UiButton.h"
#include "../../Data/Comanda.h"
class Numerobutton : public UiButton
{



	Comanda* c;
public: 
	Numerobutton(UIManager* uim, Game* game, string texturename, int x, int y, int w, int h);
	~Numerobutton();
	void execute() override;
	void update() override;
};