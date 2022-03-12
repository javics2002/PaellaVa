#include "CustomizationButton.h"
#include "../../Control/game.h"
#include "../../Control/ObjectManager.h";
#include "../../GameObjects/UI/UIManager.h"
#include "../../Scenes/Menu.h"

CustomizationButton::CustomizationButton(Game* game, string claveTextura, int x, int y, int w, int h) : UiButton(game, claveTextura, x, y, w, h)
{
    gamet = game;


}

CustomizationButton::~CustomizationButton()
{
}

void CustomizationButton::execute()
{
    uim = gamet->getUIManager();
    //cambio de escena a opciones
}