#pragma once
#include "UiButton.h"
class Game;
class UIManager;
class ExitButton :public UiButton
{
    UIManager* uim;

public:
    ExitButton(Game* game, string claveTextura, int x, int y, int w, int h);
    ~ExitButton();
    void execute()override;
};