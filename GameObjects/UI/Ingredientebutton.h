#pragma once
#include "UiButton.h"
#include "../../Data/Comanda.h"
class UIManager;
class Ingredientebutton:public UiButton
{
	UIManager* uim;
	
public:
	Ingredientebutton(UIManager* uim,Game* game, string texturename, int x, int y, int w, int h);
	~Ingredientebutton();
	void execute() override;
	void update() override;

};

