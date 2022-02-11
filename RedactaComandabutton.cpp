#include "RedactaComandabutton.h"
#include "Control/Game.h"
RedactaComandabutton::RedactaComandabutton(Game* game, TextureName texturename, int x, int y, int w, int h):UiButton(game,texturename,x,y,w,h)
{
	gamet = game;

}
RedactaComandabutton::~RedactaComandabutton()
{
}
void RedactaComandabutton::execute()
{
	gamet->getObjectManager()->creaComanda(game);
}
