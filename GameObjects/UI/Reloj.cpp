#include "Reloj.h"
#include "../../Scenes/GameOver.h"
#include "../../Scenes/Jornada.h"
Reloj::Reloj(Game* mGame, int numeroJornada) : GameObject(mGame)
{
	this->mGame = mGame;
	setDimension(w, h);
	setPosition(sdlutils().width() - getWidth(), getHeight());
	setActive(true);
	mLastUpdate = 0;

	relojTexture = &sdlutils().images().at("reloj");

	if (numeroJornada != 0) {
		hourIni--;
		minuteIni += 50;	
	}

	currentTime.hours = hourIni;
	currentTime.minutes = minuteIni;
		
	
	string timeText = parseTimeToString(currentTime.hours, currentTime.minutes);
	setTexture(timeText, string("paella"), fgColor, bgColor);

	ultimaHora = false;

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
	return (hourFin <= currentTime.hours && minuteFin <= currentTime.minutes);
}

void Reloj::render(SDL_Rect* cameraRect)
{
	SDL_Rect clock = { getTexBox().x-20, getTexBox().y-20, getTexBox().w+40, getTexBox().h+40 };
	drawRender(clock, relojTexture);
	drawRender(cameraRect);
}


void Reloj::update()
{
	if (mLastUpdate + updateTime_ > sdlutils().virtualTimer().currTime()) { //si no pasan
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
		currentTime.minutes += addedMinutes;

		if (currentTime.minutes >= 60) {
			currentTime.hours += currentTime.minutes / 60;
			currentTime.minutes = currentTime.minutes % 60;
		}

		//Si queda justo una hora para cerrar avisamos
		if (!ultimaHora && hourFin - 1 == currentTime.hours && minuteFin == currentTime.minutes) {
			ultimaHora = true;
			sdlutils().soundEffects().at("ultimaHora").play();
		}

		if (currentTime.hours >= 24) {
			currentTime.hours = currentTime.hours % 24;
		}
		string timeText = parseTimeToString(currentTime.hours, currentTime.minutes);
		setTexture(timeText, string("paella"), fgColor, bgColor);

		

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
