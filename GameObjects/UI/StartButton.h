#pragma once
#include "UiButton.h"
#include "../../Data/Comanda.h"
class Game;
class UIManager;
class StartButton :public UiButton
{
    Game* gamet;
    UIManager* uim;


public:
    StartButton(Game* game, string claveTextura, int x, int y, int w, int h);
    ~StartButton();
    void execute()override;
};