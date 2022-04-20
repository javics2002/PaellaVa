#include "Lavavajillas.h"
#include "../../Scenes/Tutorial.h"

Lavavajillas::Lavavajillas(Game* game, Vector2D<double> pos) : Mueble(game, pos, TILE_SIZE, TILE_SIZE, "lavavajillas")
{
	clip.w = timerTexture->width() / 8;
	clip.h = timerTexture->height();
	clip.y = 0;
}

void Lavavajillas::update()
{
	if (!paellasSucias.empty() && isActive())
		lavando();
}

void Lavavajillas::lavando()
{

	if (sdlutils().currRealTime() - initTime >= TIEMPO_LAVADO) {
		if (paellasLimpias.empty()) {
			paellasSucias.front()->setPosition(getRectCenter(getOverlap()));
		}
		paellasSucias.front()->finLavado();
		paellasLimpias.push_back(paellasSucias.front());
		paellasSucias.pop_front();
		

		initTime = sdlutils().currRealTime();		
	}

	else if (sdlutils().currRealTime() - initTime >= rellenoTimer + TIEMPO_LAVADO / 8) {

		clip.x = i * clip.w;

		i++;

		rellenoTimer += TIEMPO_LAVADO / 8;
	}
}

bool Lavavajillas::receivePaella(Paella* paella_)
{
	if (paella_->getContenido() == Sucia) {


		if (dynamic_cast<Tutorial*>(game->getCurrentScene()) && game->getCurrentScene()->getState() == dejarLavavajillas)
			game->getCurrentScene()->changeState(pausaDejarLavavajillas);

		initTime = sdlutils().currRealTime();

		paellasSucias.push_back(paella_);

		paella_->setPosition(Vector2D<double>(-500, 0));
		paella_->iniLavado();

		return true;
	}

	return false;
}

bool Lavavajillas::returnObject(Player* p)
{
	if (!paellasLimpias.empty())
	{
		//TOCHECK: Podr�amos hacer un return del objeto y que el player se lo guarde a s� mismo
		p->setPickedObject(paellasLimpias.front(), PAELLA);

		paellasLimpias.pop_front();

		if (!paellasLimpias.empty())
			paellasLimpias.front()->setPosition(getRectCenter(getOverlap()));


		if (dynamic_cast<Tutorial*>(game->getCurrentScene()) && game->getCurrentScene()->getState() == cogerLavavajillas)
			game->getCurrentScene()->changeState(pausaCogerLavavajillas);


		i = 0;
		clip.x = 0;
		rellenoTimer = 0;

		return true;
	}
	else
		return false;
}

void Lavavajillas::render(SDL_Rect* camera)
{
	if (isActive()) {
		SDL_Rect dest = { getX() - getWidth() / 2, getY() - getHeight() / 2, getWidth(),
			getHeight() };

		drawRender(camera, dest, &sdlutils().images().at("lavavajillas"));

		if (!paellasSucias.empty() && paellasSucias.front()->getContenido() == Sucia && i != 0) {

			SDL_Rect dest_ = { getX() + getWidth() / 2, getY() - getHeight() / 2, timerDimension, timerDimension };

			drawRender(camera, dest_, &sdlutils().images().at("timer"), clip);
		}

		if (!paellasLimpias.empty()) {
			(*paellasLimpias.begin())->render(camera);
		}
	}
}

SDL_Rect Lavavajillas::getOverlap()
{
	if(isActive())return getTexBox();
	return { 0,0,0,0 };
}

SDL_Rect Lavavajillas::getCollider()
{
	if (isActive())return getTexBox();
	return { 0,0,0,0 };
}

