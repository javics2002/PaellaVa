#include "Fogon.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"
#include "../Paella.h"
#include "../../sdlutils/SDLUtils.h"

Fogon::Fogon(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "fogon")
{
	paella_ = nullptr;
	setColliderRect({ (int)getX(), (int)getY() - h/4, w, h / 2 });
}


void Fogon::update()
{
}

void Fogon::render(SDL_Rect* cameraRect)
{
	SDL_Rect dest = { getX() - getWidth() / 2, getY() - getHeight() / 2, getWidth(),
	getHeight() };


	drawRender(cameraRect, dest, &sdlutils().images().at("fogon"));

	if (paella_!= nullptr) {

		SDL_Rect dest_ = { getX() - getWidth() / 2, getY() + getHeight() / 2 - barraCoccionY*15, barraCoccionX, barraCoccionY };

		drawRender(cameraRect, dest_, &sdlutils().images().at("barraCoccion"));
		if (flechaCoccionPosX + flechaCoccionX / 2 <  getX() - getWidth() / 2 + barraCoccionX - flechaCoccionX / 2)
			flechaCoccionPosX += movimientoFlecha;
		else flechaCoccionPosX = getX() - getWidth() / 2 + barraCoccionX - flechaCoccionX/2 ;

		SDL_Rect dest_1 = {  flechaCoccionPosX , getY() + getHeight() / 2 - barraCoccionY * 15 - flechaCoccionY, flechaCoccionX, flechaCoccionY };

		drawRender(cameraRect, dest_1, &sdlutils().images().at("flechaCoccion"));
	}
}

bool Fogon::receivePaella(Paella* pa)
{
	//Si ya tiene objeto, no recoge objeto
	if (paella_ == nullptr 
		&& pa->getState() == Preparacion
			&& pa->conArroz())
	{
		paella_ = pa;

		paella_->setPosition(getX(), getY() - 3 * pa->getHeight() / 4);

		//empezar a cocer la paella
		paella_->setState(Coccion);



		sdlutils().soundEffects().at("enciendeFogon").play();
		canalSonido = sdlutils().soundEffects().at("fogon").play(-1);

		return true;
	}
	else
		return false;
}

bool Fogon::returnObject(Player* p)
{
	if (paella_ != nullptr)
	{
		paella_->setState(Hecha);

		p->setPickedObject(paella_, PAELLA);

		paella_ = nullptr;

		//Paramos el sonido
		sdlutils().soundEffects().at("fogon").haltChannel(canalSonido);

		//TOCHECK: Habría que devolver la paella al estado de "Preparación" si no está Preparada?
		//Y desde donde se llama a que esté ya Preparada?

		return true;
	}
	else
		return false;
}

