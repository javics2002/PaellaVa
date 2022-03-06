#pragma once
#include "UiButton.h"
#include "../../Data/Comanda.h"
class Ingredientebutton:public UiButton
{
	Comanda*  comandat;
	Game* gamet;
	string textuname;
public:
	Ingredientebutton(Comanda* comanda,Game* game, string texturename, int x, int y, int w, int h);
	~Ingredientebutton();
	void execute() override;

};

