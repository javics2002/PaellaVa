#include "Fogon.h"
#include "../UI/UIManager.h"
#include "../../Control/Game.h"
#include "../../Control/ObjectManager.h"
#include "../Paella.h"
#include "../../sdlutils/SDLUtils.h"
#include "../../Scenes/Tutorial.h"
#include "../../Utils/ParticleExample.h"

using tweeny::easing;

Fogon::Fogon(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "fogon")
{
	funcionando = true;
	paella_ = nullptr;
}


void Fogon::update()
{
	if(!funcionando && paella_!=nullptr)
		paella_->setState(Hecha);
;	if (funcionando && couldBreak <= 0)
	{
		testMueble();
		if (funcionando){
			//se reduce cuando se podría romper
			couldBreak = MAX_BREAK_TIME - REDUCE_BREAK_TIME;
		}
		else{
			//se resetea cuando se podría romper
			couldBreak = MAX_BREAK_TIME;
		}
	}
	else if (funcionando && couldBreak > 0){
		couldBreak -= seg;
	}
	if (!funcionando)
		humo->setStyle(ParticleExample::EXPLOSION);
	else humo->setStyle(ParticleExample::NONE);
	humo->setPosition(getX(), getY());
	humo->update();

}

void Fogon::render(SDL_Rect* cameraRect)
{
	//Si no funciona usar la textura del fuego

	SDL_Rect dest = { getX() - getWidth() / 2, getY() - getHeight() / 2, getWidth(),
	getHeight() };

	if (isActive()) {

		if(funcionando)drawRender(cameraRect, dest, &sdlutils().images().at("fogon"));
		else drawRender(cameraRect, dest, &sdlutils().images().at("berenjena"));


		if (paella_ != nullptr && barra) {

			SDL_Rect dest_ = { getX() - barraCoccionX / 2 , getY() - getHeight() / 1.5, barraCoccionX, barraCoccionY };

			drawRender(cameraRect, dest_, &sdlutils().images().at("barraCoccion"));
			drawRender(cameraRect, dest_1, &sdlutils().images().at("flechaCoccion"));
		}
		humo->draw(cameraRect);
	}
}

bool Fogon::receivePaella(Paella* pa)
{
	if (funcionando)
	{
		//Si ya tiene objeto, no recoge objeto
		if (paella_ == nullptr
			&& pa->getState() == Preparacion
			&& pa->conArroz())
		{
			paella_ = pa;

			paella_->setPosition(getRectCenter(getOverlap()));

			//empezar a cocer la paella
			paella_->setState(Coccion);

			barra = true;

			game->getUIManager()->addTween((float)(getX() - barraCoccionX / 2 - flechaCoccionX / 2), (float)(getX() + barraCoccionX / 2 - flechaCoccionX / 2), tiempoDeCoccion, true)
				.onStep(
					[this](tweeny::tween<float>& t, float) mutable {
						dest_1.x = t.peek();
						if (t.progress() == 1 || paella_ == nullptr || !funcionando) {

							barra = false;

							return true;
						}
						return false;
					});


			sdlutils().soundEffects().at("enciendeFogon").play();
			canalSonido = sdlutils().soundEffects().at("fogon").play(-1);

			return true;
		}
	}
	return false;
}

bool Fogon::returnObject(Player* p)
{
	if (paella_ != nullptr)
	{
		paella_->setState(Hecha);

		//Si nos hemos pasado, nos quemamos
		if (paella_->getCoccion() >= Quemada) {
			game->getUIManager()->quemarse();
		}

		if (dynamic_cast<Tutorial*>(game->getCurrentScene()) && game->getCurrentScene()->getState() == States::recogerPaellaCocinada)
			game->getCurrentScene()->changeState(States::pausaRecogerPaellaCocinada);

		p->setPickedObject(paella_, PAELLA);

		paella_ = nullptr;
		barra = false;
		dest_1 = { (int)getX() - barraCoccionX / 2 + flechaCoccionX / 2,(int)getY() - (int)(getHeight() / 1.5) - (int)(barraCoccionY / 1.5),
				flechaCoccionX, flechaCoccionY 
		};

		return true;
	}
	else
		return false;
}

