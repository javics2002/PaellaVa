#include "Lavavajillas.h"
#include "../../Utils/ParticleExample.h"
#include "../../Scenes/Tutorial.h"

Lavavajillas::Lavavajillas(Game* mGame, Vector2D<double> pos) : Mueble(mGame, pos, TILE_SIZE, TILE_SIZE, "lavavajillas")
{
	clip.w = timerTexture->width() / 8;
	clip.h = timerTexture->height();
	clip.y = 0;
}

void Lavavajillas::update()
{
	if (!funcionando) // Si deja de funcionar se activa el humo
		humo->setStyle(ParticleExample::SMOKE);
	else
		humo->setStyle(ParticleExample::NONE);

	humo->setPosition(getRectCenter(getOverlap()).getX(), getRectCenter(getOverlap()).getY());
	humo->update();

	if (funcionando && !mPaellasSucias.empty() && isActive()) // Si hay paellas sucias, empiezas a lavar la que se ha metido primero
		lavando();
	else if (!funcionando && !mPaellasSucias.empty() && isActive()) // Si ya se ha lavado se desactiva la animacion y se resetea el contador
	{
		i = 0;
		clip.x = 0;
		rellenoTimer = 0;

		mInitTime = sdlutils().currRealTime();
	}


	if (!mGame->getNetworkManager()->isHost())
		return;

	if (funcionando && mCouldBreak <= 0 ) // Si esta funcionando
	{
		testMueble();
		if (funcionando)
		{
			//se reduce cuando se podr�a romper
			mCouldBreak = MAX_BREAK_TIME - REDUCE_BREAK_TIME;
		}
		else
		{
			//se resetea cuando se podr�a romper
			mCouldBreak = MAX_BREAK_TIME;
			humo->setStyle(ParticleExample::EXPLOSION);
		}
	}
	else if (funcionando && mCouldBreak > 0)
	{
		mCouldBreak -= seg;
	}
}


void Lavavajillas::lavando() // Mientras no haya acabado el tiempo de lavado, suma al contador y avanza la animacion
{
	if (sdlutils().currRealTime() - mInitTime >= TIEMPO_LAVADO) { // Cuando ha terminado de lavarse, pasa de estar sucia a limpia y aparece arriba
		if (mPaellasLimpias.empty()) {
			mPaellasSucias.front()->setPosition(getRectCenter(getOverlap()));
		}
		mPaellasSucias.front()->finLavado();
		mPaellasLimpias.push_back(mPaellasSucias.front());
		mPaellasSucias.pop_front();

		i = 0;
		clip.x = 0;
		rellenoTimer = 0;

		mInitTime = sdlutils().currRealTime();
	}
	else if (sdlutils().currRealTime() - mInitTime >= rellenoTimer + TIEMPO_LAVADO / 8) {

		clip.x = i * clip.w;

		i++;

		rellenoTimer += TIEMPO_LAVADO / 8;
	}
}

bool Lavavajillas::receivePaella(Paella* paella_)
{
	if (paella_ != nullptr && funcionando) // SI no hay paella y funciona
	{
		if (paella_->getContenido() == Sucia) { // Si la paella esta sucia

			
			//Para el tutorial, cambiamos de estado
			if (dynamic_cast<Tutorial*>(mGame->getCurrentScene()) && mGame->getCurrentScene()->getState() == TUTORIALSTATE_DEJAR_LAVAVAJILLAS)
				mGame->getCurrentScene()->changeState(TUTORIALSTATE_PAUSA_DEJAR_LAVAVAJILLAS);

			mInitTime = sdlutils().currRealTime();

			mPaellasSucias.push_back(paella_);

			paella_->setPosition(Vector2D<double>(-500, 0));
			paella_->iniLavado();

			sdlutils().soundEffects().at("paellaLavavajillas").play();
			sdlutils().soundEffects().at("lavarPaella").play();
			return true;
		}
	}

	return false;
}

bool Lavavajillas::returnObject(Player* p)
{
	if (mPaellasLimpias.empty() || p->getPickedPaellasCount() > p->getMaxPickedPaellasCount() || (p->getPickedObject() != nullptr && p->getPickedPaellasCount() == 0)) // Si no hay paellas limpias
		return false;

	p->setPickedObject(mPaellasLimpias.front(), PAELLA);

	mPaellasLimpias.pop_front();

	if (!mPaellasLimpias.empty())
		mPaellasLimpias.front()->setPosition(getRectCenter(getOverlap()));


	// Cambiamos el estado en el tutorial
	if (dynamic_cast<Tutorial*>(mGame->getCurrentScene()) && mGame->getCurrentScene()->getState() == TUTORIALSTATE_COGER_LAVAVAJILLAS)
		mGame->getCurrentScene()->changeState(TUTORIALSTATE_PAUSA_COGER_LAVAVAJILLAS);


	i = 0;
	clip.x = 0;
	rellenoTimer = 0;

	return true;
		
}

void Lavavajillas::render(SDL_Rect* camera)
{
	if (isActive()) {
		SDL_Rect dest = { getX() - getWidth() / 2, getY() - getHeight() / 2, getWidth(),
			getHeight() };

		drawRender(camera, dest, &sdlutils().images().at("lavavajillas"));
		//else drawRender(camera, dest, &sdlutils().images().at("berenjena"));

		if (!mPaellasSucias.empty() && mPaellasSucias.front()->getContenido() == Sucia && i != 0) {

			SDL_Rect dest_ = { getX() + getWidth() / 2, getY() - getHeight() / 2, timerDimension, timerDimension };

			drawRender(camera, dest_, &sdlutils().images().at("timer"), clip);
		}

		if (!mPaellasLimpias.empty()) {
			(*mPaellasLimpias.begin())->render(camera);
		}
		humo->draw(camera);
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

bool Lavavajillas::resetCounter()
{
	mCouldBreak = MAX_BREAK_TIME;
	return true;
}

