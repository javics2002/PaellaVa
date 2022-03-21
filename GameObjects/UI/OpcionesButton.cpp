#include "OpcionesButton.h"
#include "../../Control/game.h"
#include "../../Control/ObjectManager.h";
#include "../../GameObjects/UI/UIManager.h"
#include "../../Scenes/Menu.h"

OpcionesButton::OpcionesButton(Game* game, string claveTextura, int x, int y, int w, int h) : UiButton(game, claveTextura, x, y, w, h)
{
    this->game = game;
}

OpcionesButton::~OpcionesButton()
{
}

void OpcionesButton::execute()
{
    sdlutils().soundEffects().at("select").play(0, game->UI);
    //cambio de escena a opciones
}