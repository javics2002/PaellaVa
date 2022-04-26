#include "TablaProcesado.h"
#include "../../sdlutils/SDLUtils.h"

#include "../../GameObjects/Player.h"
#include "../../Control/Game.h"
#include "../../Scenes/Tutorial.h"
#include "../../Utils/ParticleExample.h"


TablaProcesado::TablaProcesado(Game* game_, Vector2D<double> pos) : Mueble(game_, pos, TILE_SIZE, 2 * TILE_SIZE, "tablaProcesado")
{
	funcionando = true;
	clip.w = timerTexture->width() / 8;
	clip.h = timerTexture->height();
	clip.y = 0;
}

void TablaProcesado::update() 
{
	if (!funcionando)
	{
		humo->setStyle(ParticleExample::SMOKE);
	}
	else
	{
		humo->setStyle(ParticleExample::NONE);
	}

	if (funcionando && ingr_ != nullptr) 
	{
		procesando();
	}
	else if (!funcionando && ingr_ != nullptr) 
	{
		i = 0;
		clip.x = 0;
		rellenoTimer = 0;
		tiempo = sdlutils().currRealTime();
	}

	if (funcionando)
	{
		if (couldBreak <= 0)
		{
			testMueble();
			if (funcionando)
			{	
				//se reduce cuando se podr�a romper
				couldBreak = MAX_BREAK_TIME - REDUCE_BREAK_TIME;
			}
			else
			{
				//se resetea cuando se podr�a romper
				couldBreak = MAX_BREAK_TIME;
				humo->setStyle(ParticleExample::EXPLOSION);
			}
		}
		else if (couldBreak > 0){
			couldBreak -= seg;
		}
	}

	humo->setPosition(getX(), getY());
	humo->update();
}

void TablaProcesado::procesando() {

	if (sdlutils().virtualTimer().currTime() - tiempo >= TIEMPO_PROCESADO) {
		ingr_->setProcesado(true, ingr_);
		tiempo = sdlutils().currRealTime();
		clip.x = i * clip.w;
		sdlutils().soundEffects().at("cortar1").haltChannel(canalSonido);
	}

	else if (sdlutils().virtualTimer().currTime() - tiempo >= rellenoTimer + TIEMPO_PROCESADO / 8) {

			clip.x = i * clip.w;
			i++;
			rellenoTimer += TIEMPO_PROCESADO / 8;
	}
}

void TablaProcesado::render(SDL_Rect* camera)
{
	if (isActive()) {
		SDL_Rect dest = { getX() - getWidth() / 2, getY() - getHeight() / 2, getWidth(),
			getHeight() };

		//if(funcionando)
			drawRender(camera, dest, &sdlutils().images().at("tablaProcesado"));
		//else drawRender(camera, dest, &sdlutils().images().at("berenjena"));

		if (ingr_ != nullptr && !ingr_->getProcesado() && i != 0) {

			SDL_Rect dest_ = { getX() - getWidth() / 2, getY() + getHeight() / 2, timerDimension, timerDimension };

			drawRender(camera, dest_, &sdlutils().images().at("timer"), clip);
		}

		humo->draw(camera);
	}

}

bool TablaProcesado::receiveIngrediente(Ingrediente* ingr)
{
	if (funcionando)
	{
		if (ingr_ == nullptr && !ingr->getProcesado()) 
		{

			ingr_ = ingr;

			tiempo = sdlutils().virtualTimer().currTime();

			ingr_->setPosition(getRectCenter(getOverlap()));

			canalSonido = sdlutils().soundEffects().at("cortar" + to_string(sdlutils().rand().nextInt(1, 4))).play(-1);

			return true;
		}
	}

	return false;
}

bool TablaProcesado::returnObject(Player* p)
{
	if (ingr_ != nullptr)
	{
		//TOCHECK: Podr�amos hacer un return del objeto y que el player se lo guarde a s� mismo
		p->setPickedObject(ingr_, INGREDIENTE);
		if (ingr_->getProcesado() && dynamic_cast<Tutorial*>(game->getCurrentScene()) && game->getCurrentScene()->getState() == States::procesarIngrediente)
			game->getCurrentScene()->changeState(States::pausaProcesarIngrediente);

		ingr_ = nullptr;

		i = 0;
		clip.x = 0;
		rellenoTimer = 0;

		sdlutils().soundEffects().at("cortar1").haltChannel(canalSonido);

		return true;
	}
	else
		return false;
}
