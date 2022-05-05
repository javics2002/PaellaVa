#include "Reloj.h"
#include "../../Scenes/GameOver.h"
#include "../../Scenes/Jornada.h"
Reloj::Reloj(Game* mGame, int numeroJornada) : GameObject(mGame)
{
	this->mGame = mGame;
	setDimension(mWidth, mHeight);
	setPosition(sdlutils().width() - getWidth(), getHeight());
	setActive(true);
	mLastUpdate = 0;

	mRelojTexture = &sdlutils().images().at("reloj");

	if (numeroJornada != 0) {
		mHourIni--;
		mMinuteIni += 50;	
	}

	mCurrentTime.hours = mHourIni;
	mCurrentTime.minutes = mMinuteIni;
		
	
	string timeText = parseTimeToString(mCurrentTime.hours, mCurrentTime.minutes);
	setTexture(timeText, string("paella"), FG_COLOR, BG_COLOR);

	mUltimaHora = false;

	mNumeroJornada = numeroJornada;
}

Reloj::~Reloj()
{
}

bool Reloj::finDia()
{
	if (!mGame->getNetworkManager()->isHost())
		return false;
	//return SDL_TICKS_PASSED(SDL_GetTicks(), totalJornada);
	return (mHourFin <= mCurrentTime.hours && mMinuteFin <= mCurrentTime.minutes);
}

void Reloj::render(SDL_Rect* cameraRect)
{
	SDL_Rect clock = { getTexBox().x-20, getTexBox().y-20, getTexBox().w+40, getTexBox().h+40 };
	drawRender(clock, mRelojTexture);
	drawRender(cameraRect);
}


void Reloj::update()
{
	if (mLastUpdate + mUpdateTime > sdlutils().virtualTimer().currTime()) { //si no pasan
		return;
	}

	

	if (finDia())
	{
		mGame->getNetworkManager()->setGameStarted(false);

		mGame->sendMessageScene(new GameOver(mGame, 0, mNumeroJornada));
		mGame->getNetworkManager()->sendFinishGame(dynamic_cast<Jornada*>(mGame->getCurrentScene())->getPunctuationJornada(), mNumeroJornada);
	}
	else
	{
		mLastUpdate = sdlutils().virtualTimer().currTime();

		//1 minuto en Ticks = 1 hora en el juego
		mCurrentTime.minutes += mAddedMinutes;

		if (mCurrentTime.minutes >= 60) {
			mCurrentTime.hours += mCurrentTime.minutes / 60;
			mCurrentTime.minutes = mCurrentTime.minutes % 60;
		}

		//Si queda justo una hora para cerrar avisamos
		if (!mUltimaHora && mHourFin - 1 == mCurrentTime.hours && mMinuteFin == mCurrentTime.minutes) {
			mUltimaHora = true;
			sdlutils().soundEffects().at("ultimaHora").play();
		}

		if (mCurrentTime.hours >= 24) {
			mCurrentTime.hours = mCurrentTime.hours % 24;
		}
		string timeText = parseTimeToString(mCurrentTime.hours, mCurrentTime.minutes);
		setTexture(timeText, string("paella"), FG_COLOR, BG_COLOR);

		

	}

		
	
}

string Reloj::parseTimeToString(int hours, int minutes)
{
	string timeText = "";
	if (hours < 10) timeText += "0";
	timeText += to_string(hours);

	timeText += ":";

	if (minutes < 10) timeText += "0";
	timeText += to_string(minutes);
	return timeText;
}
