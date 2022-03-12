#pragma once
#include "UiButton.h"
class Game;
class UIManager;
class OpcionesButton :public UiButton
{
    Game* gamet;
    UIManager* uim;


public:
    OpcionesButton(Game* game, string claveTextura, int x, int y, int w, int h);
    ~OpcionesButton();
    void execute()override;
};