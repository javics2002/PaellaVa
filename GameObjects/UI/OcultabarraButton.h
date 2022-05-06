#pragma once
#include "UiButton.h"
class ListaComandas;
class OcultabarraButton : public UiButton
{
	ListaComandas* mlista;
public:
	OcultabarraButton(UIManager* uim, Game* mGame, string texturename, int x, int y, int w, int h,ListaComandas* l);
	~OcultabarraButton();
	void execute(bool& exit) override;
};

