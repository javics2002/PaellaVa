#pragma once
#include "UiButton.h"
#include "../../Data/Comanda.h"

class TamanoButton: public UiButton
{
	Comanda* c;
	string textura;
public:
	TamanoButton(UIManager* uim, Game* mGame, string texturename, int x, int y, int w, int h,int id);
	~TamanoButton();
	void execute(bool& exit) override;
	void update() override;
	int idonline;
};

