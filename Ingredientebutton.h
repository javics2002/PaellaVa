#pragma once
#include "UiButton.h"
#include "Data/Comanda.h"
class Ingredientebutton:public UiButton
{
	Comanda*  comandat;
	Game* gamet;
	TextureName textuname;
public:
	Ingredientebutton(Comanda* comanda,Game* game, TextureName texturename, int x, int y, int w, int h);
	~Ingredientebutton();
	void execute() override;

};

