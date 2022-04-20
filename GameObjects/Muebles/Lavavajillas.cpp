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
	if (!pilaPaellas.empty() && isActive())
	{
		if (funcionando)
		{
			lavando();
		}
		else if (!funcionando && !pilaPaellas.empty())
		{
			//Si el lavavajillas est� roto, se ensucian las paellas
			ensuciarPaellas();
		}
	}

	if (funcionando && couldBreak <= 0)
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
		}
	}
	else if (funcionando && couldBreak > 0)
	{
		couldBreak -= seg;
	}
}

void Lavavajillas::ensuciarPaellas()
{
	for (auto it = paellasLimpias.front(); it != paellasLimpias.back(); it++)
	{
		pilaPaellas.push_back(pilaPaellas.front());
		paellasLimpias.pop_front();
	}

	for (auto it = pilaPaellas.front(); it != pilaPaellas.back(); it++)
	{
		it->setTexture("paellaSucia");
		it->setContenido(Sucia);
		it->setEnsuciada();
	}
}

void Lavavajillas::lavando()
{
	if (funcionando)
	{
		if (sdlutils().currRealTime() - initTime >= TIEMPO_LAVADO) {
			pilaPaellas.front()->setLavado(Limpia, "paellaLimpia");
			paellasLimpias.push_back(pilaPaellas.front());
			pilaPaellas.pop_front();
			initTime = sdlutils().currRealTime();
		}

		else if (sdlutils().currRealTime() - initTime >= rellenoTimer + TIEMPO_LAVADO / 8) {

			clip.x = i * clip.w;

			i++;

			rellenoTimer += TIEMPO_LAVADO / 8;
		}
	}
}

bool Lavavajillas::receivePaella(Paella* paella_)
{
	if (funcionando)
	{
		if (paella_->getContenido() == Sucia) {


		if (dynamic_cast<Tutorial*>(game->getCurrentScene()) && game->getCurrentScene()->getState() == dejarLavavajillas)
			game->getCurrentScene()->changeState(pausaDejarLavavajillas);

		pilaPaellas.push_back(paella_);

			paella_->setPosition(getRectCenter(getOverlap()));

			initTime = sdlutils().currRealTime();

			return true;
		}
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

	//Si no funciona usar la textura del fuego

		drawRender(camera, dest, &sdlutils().images().at("lavavajillas"));

		if (!pilaPaellas.empty() && pilaPaellas.front()->getContenido() == Sucia && i != 0) {

			SDL_Rect dest_ = { getX() + getWidth() / 2, getY() - getHeight() / 2, timerDimension, timerDimension };

			drawRender(camera, dest_, &sdlutils().images().at("timer"), clip);
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

