#pragma once
#include "UiButton.h"
class Game;
class UIManager;
class CustomizationButton :public UiButton
{
    Game* gamet;
    UIManager* uim;


public:
    CustomizationButton(Game* game, string claveTextura, int x, int y, int w, int h);
    ~CustomizationButton();
    void execute()override;
};

