#include "EliminaComandaButton.h"
#include "UIManager.h"
#include "../../Control/Game.h"
#include "../../Control/NetworkManager.h"
#include "../../Data/ListaComandas.h"
EliminaComandaButton::EliminaComandaButton(UIManager* u,Comanda* c, Game* game, string texturename, int x, int y, int w, int h) : UiButton(game, texturename, x, y, w, h)
{
	comandat = c;
	uimt = u;
}
EliminaComandaButton::~EliminaComandaButton()
{
}
void EliminaComandaButton::execute(bool& exit)
{
	if (isActive())
	{
		//uimt->getBarra()->borralaComandaQueteMandan(uimt->getBarra()->queComandaBorro());
		game->getNetworkManager()->syncBarra(uimt->getBarra()->queComandaBorro());
		uimt->getBarra()->finalizacomanda(comandat);
		
		sdlutils().soundEffects().at("comandaDescartada").play(0, game->UI);
	}
}
void EliminaComandaButton::update()
{
}