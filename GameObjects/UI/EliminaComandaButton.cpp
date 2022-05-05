#include "EliminaComandaButton.h"
#include "UIManager.h"
#include "../../Control/Game.h"
#include "../../Control/NetworkManager.h"
#include "../../Data/ListaComandas.h"
EliminaComandaButton::EliminaComandaButton(UIManager* u,Comanda* c, Game* mGame, string texturename, int x, int y, int w, int h) : UiButton(mGame, texturename, x, y, w, h)
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
		mGame->getNetworkManager()->syncBarra(uimt->getBarra()->queComandaBorro());
		uimt->getBarra()->finalizacomanda(comandat);
		
		sdlutils().soundEffects().at("comandaDescartada").play(0, mGame->UI);
	}
}
void EliminaComandaButton::update()
{
}