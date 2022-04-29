#include "Ventanilla.h"
#include "../../Control/Game.h"
#include "../UI/UIManager.h"
#include "../../Data/ListaComandas.h"
#include "../../Data/Comanda.h"
#include "../../sdlutils/SDLUtils.h"
#include "../Paella.h"
#include "../../Scenes/Tutorial.h"

Ventanilla::Ventanilla(Game* game, Vector2D<double> pos, SDL_Rect* r) : Mueble(game, pos, 1 * TILE_SIZE, 2 * TILE_SIZE, "encimera")
{
	//uimt = game->getUIManager(); lpm aqui da null
	camerar = r;
	xnumero = pos.getX()-getWidth();
	ynumero = pos.getY()+getHeight();

	//Siempre tiene que funcionar
	funcionando = true;
}
bool Ventanilla::receivePaella(Paella* pa)
{
	uimt = game->getUIManager();
	if (pa != nullptr)
	{
		mpaella = pa;
		mpaella->setPosition(getRectCenter(getOverlap()));

		ih().setKey(false, InputHandler::A);
		uimt->creaComandaVentanilla(game);
		
		if (dynamic_cast<Tutorial*>(game->getCurrentScene()) && game->getCurrentScene()->getState() == States::dejarPaellaVentanilla) {
			game->getCurrentScene()->changeState(States::pausaDejarPaellVentanilla);
		}

		return true;
	}

	return false;
}

void Ventanilla::render(SDL_Rect* cameraRect)
{
	SDL_Rect rect = {};
	drawRender(cameraRect);
	rect = {(int) getPosition().getX() - 50, (int)getPosition().getY() - 50,50,50 };
	if(numactex!=nullptr)
		drawRender(cameraRect, rect, numactex);
}
bool Ventanilla::returnObject(Player* p)
{
	if (mpaella != nullptr && numactex!=nullptr)
	{
		p->setPickedObject(mpaella, PAELLA);

		if (dynamic_cast<Tutorial*>(game->getCurrentScene()) && game->getCurrentScene()->getState() == States::cogerPaellaVentanilla)
			game->getCurrentScene()->changeState(States::pausaCogerPaellaVentanilla);

		mpaella = nullptr;
		numactex = nullptr;

		return true;
	}
	else
		return false;
}

void Ventanilla::receiveNumeroMesa(Texture* t)
{
	if (mpaella != nullptr) {
		numactex = t;
		numero = true;
		game->getObjectManager()->getPlayerOne()->changePlayer(false);
	}
}
