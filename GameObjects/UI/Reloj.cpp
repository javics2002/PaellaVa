#include "Reloj.h"
#include "../../Scenes/GameOver.h"

Reloj::Reloj(Game* game) : GameObject(game)
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
	drawRender(getTexCollider(), relojTexture);
	drawRender(cameraRect);
}


void Reloj::update()
{
	if (SDL_GetTicks() - (mLastUpdate - offsetTime) >= updateTime_)
	{
		if (finDia())
		{
#ifndef _DEBUG
			game->changeScene(new GameOver(game, 100));
#endif // _DEBUG
		}
		//Si queda justo una hora para cerrar avisamos
		else if (!ultimaHora && hourFin - 1 == currentTime.hours && minuteFin == currentTime.minutes) {
			ultimaHora = true;
			sdlutils().soundEffects().at("ultimaHora").play();
		}
		else
		{
			//1 minuto en Ticks = 1 hora en el juego
			currentTime.minutes += addedMinutes;

			if (currentTime.minutes >= 60) {
				currentTime.hours += currentTime.minutes / 60;
				currentTime.minutes = currentTime.minutes % 60;
			}
			if (currentTime.hours >= 24) {
				currentTime.hours = currentTime.hours % 24;
			}
			string timeText = parseTimeToString(currentTime.hours, currentTime.minutes);
			setTexture(timeText, string("paella"), fgColor, bgColor);
		}
		offsetTime = 0;
		mLastUpdate = SDL_GetTicks();

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
