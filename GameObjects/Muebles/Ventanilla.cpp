#include "Ventanilla.h"
#include "../../Control/Game.h"
#include "../UI/UIManager.h"
#include "../../Data/ListaComandas.h"
#include "../../Data/Comanda.h"
#include "../../sdlutils/SDLUtils.h"
#include "../Paella.h"
Ventanilla::Ventanilla(Game* game, Vector2D<double> pos, SDL_Rect* r) : Mueble(game, pos, 1 * TILE_SIZE, 2 * TILE_SIZE, "encimera")
{
	//uimt = game->getUIManager(); lpm aqui da null
	camerar = r;
	xnumero = pos.getX()-getWidth();
	ynumero = pos.getY()+getHeight();
}
bool Ventanilla::receivePaella(Paella* pa)
{
	uimt = game->getUIManager();
	if (pa != nullptr)
	{
		cout << "TENGO UNA PAELLA";

		mpaella = pa;
		mpaella->setPosition(getRectCenter(getOverlap()));

		if (uimt->getBarra()->getComandaSeleccionada() != nullptr)
		{
			//UiButton* u = uimt->getBarra()->getComandaSeleccionada()->getNumeromesa();
			//numeroactual = new UiButton(game, u->getTextura(), xnumero, ynumero, 80, 80);
			if (uimt->getBarra()->getComandaSeleccionada()->getNumeromesa() != nullptr) {
				string s = uimt->getBarra()->getComandaSeleccionada()->getNumeromesa()->getTextura();
				numactex = &sdlutils().images().at(s);
				lastnumact = sdlutils().currRealTime();
			}

			else//la comanda no tiene numero de mesa que salga una interrogacion
			{
				
			}
			
		}

		return true;
	}

	return false;
}
void Ventanilla::muestraNumeroMesa()
{
	if (numeroactual != nullptr)
	{

		numeroactual->render(new SDL_Rect{ (1,1,1,1) });
	}
}
void Ventanilla::render(SDL_Rect* cameraRect)
{
	SDL_Rect rect = {};
	drawRender(cameraRect);
	muestraNumeroMesa();
	rect = {(int) getPosition().getX() - 50, (int)getPosition().getY() - 50,50,50 };
	if(numactex!=nullptr)
	drawRender(cameraRect, rect, numactex);
}
bool Ventanilla::returnObject(Player* p)
{
	if (mpaella != nullptr)
	{
		p->setPickedObject(mpaella, PAELLA);

		mpaella = nullptr;

		return true;
	}
	else
		return false;
}
void Ventanilla::update()
{

	if (sdlutils().currRealTime() > lastnumact + screentimenumero)
	{
		numactex = nullptr;

	}
}