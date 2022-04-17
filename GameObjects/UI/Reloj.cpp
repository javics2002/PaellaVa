#include "Reloj.h"
#include "../../Scenes/GameOver.h"

Reloj::Reloj(Game* game, int numeroJornada) : GameObject(game)
{
	this->game = game;
	setDimension(w, h);
	setPosition(sdlutils().width() - getWidth(), getHeight());
	setActive(true);
	mLastUpdate = SDL_GetTicks();

	relojTexture = &sdlutils().images().at("reloj");

	currentTime.hours = hourIni;
	currentTime.minutes = minuteIni;
	string timeText = parseTimeToString(hourIni, minuteIni);

	setTexture(timeText, string("paella"), fgColor, bgColor);

	ultimaHora = false;

	mNumeroJornada = numeroJornada;
}

Reloj::~Reloj()
{
}

bool Reloj::finDia()
{
	//return SDL_TICKS_PASSED(SDL_GetTicks(), totalJornada);
	return (hourFin <= currentTime.hours && minuteFin <= currentTime.minutes);
}

void Reloj::render(SDL_Rect* cameraRect)
{
	drawRender(getTexBox(), relojTexture);
	drawRender(cameraRect);
}


void Reloj::update()
{
	if (mLastUpdate + updateTime_ > sdlutils().virtualTimer().currTime()) { //si no pasan
		return;
	}

	mLastUpdate = sdlutils().virtualTimer().currTime();

	if (finDia())
	{
#ifndef _DEBUG
		game->sendMessageScene(new GameOver(game, 100, mNumeroJornada));
#endif // _DEBUG
	}
	else
	{
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
