#include "TablaProcesado.h"

#include "../../GameObjects/Player.h"
#include "../../Control/Game.h"


TablaProcesado::TablaProcesado(Game* game_, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, 2 * TILE_SIZE, "tablaProcesado")
{
	clip.w = timerTexture->width() / 8;
	clip.h = timerTexture->height();
	clip.y = 0;
	setColliderRect({ (int)getX(), (int)getY() - h / 4, w, 2 * h / 3 });
}

void TablaProcesado::update() {
	if (ingr_ != nullptr) {
		procesando();
	}
}

void TablaProcesado::procesando() {
	if (sdlutils().currRealTime() - tiempo >= TIEMPO_PROCESADO) {
		ingr_->setProcesado(true, ingr_);
		tiempo = sdlutils().currRealTime();

		clip.x = i * clip.w;

		sdlutils().soundEffects().at("cortar1").haltChannel(canalSonido);
	}
	else if (sdlutils().currRealTime() - tiempo >= rellenoTimer + TIEMPO_PROCESADO / 8) {

		clip.x = i * clip.w;

		i++;

		rellenoTimer += TIEMPO_PROCESADO / 8;
	}
}

void TablaProcesado::render(SDL_Rect* camera)
{
	SDL_Rect dest = { getX() - getWidth() / 2, getY() - getHeight() / 2, getWidth(),
		getHeight() };

	drawRender(camera, dest, &sdlutils().images().at("tablaProcesado"));

	if (ingr_ != nullptr && !ingr_->getProcesado() && i != 0) {

		SDL_Rect dest_ = { getX() - getWidth() / 2, getY() + getHeight() / 2, timerDimension, timerDimension };

		drawRender(camera, dest_, &sdlutils().images().at("timer"), clip);
	}
}

bool TablaProcesado::receiveIngrediente(Ingrediente* ingr)
{
	if (ingr_ == nullptr && !ingr->getProcesado()) {

		ingr_ = ingr;

		tiempo = sdlutils().currRealTime();

		ingr_->setPosition(getX(), getY() - ingr->getHeight() / 2);

		canalSonido = sdlutils().soundEffects().at("cortar" + to_string(sdlutils().rand().nextInt(1, 4))).play(-1);

		return true;
	}

	return false;
}

bool TablaProcesado::returnObject(Player* p)
{
	if (ingr_ != nullptr)
	{
		//TOCHECK: Podríamos hacer un return del objeto y que el player se lo guarde a sí mismo
		p->setPickedObject(ingr_, INGREDIENTE);

		ingr_ = nullptr;

		i = 0;
		clip.x = 0;
		rellenoTimer = 0;

		return true;
	}
	else
		return false;
}
