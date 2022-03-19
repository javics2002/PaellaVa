#include "StartButton.h"
#include "../../Control/game.h"
#include "../../Control/ObjectManager.h";
#include "../../GameObjects/UI/UIManager.h"
#include "../../Scenes/Restaurante.h"

#include "../../Scenes/Menu.h"

StartButton::StartButton(Game* game, string claveTextura, int x, int y, int w, int h) : UiButton(game, claveTextura, x, y, w, h)
{
}

StartButton::~StartButton()
{
}

void StartButton::execute()
{
    game->changeScene(new Restaurante(game));
}