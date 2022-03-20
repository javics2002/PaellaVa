#pragma once
#include "UiButton.h"
#include "../../Data/Comanda.h"
class UIManager;
class EliminaComandaButton: public UiButton
{
	Comanda* comandat;
	UIManager* uimt;
public:
	EliminaComandaButton(UIManager* u,Comanda* c, Game* game, string texturename, int x, int y, int w, int h);
	~EliminaComandaButton();
	void execute() override;
	void update() override;

};

