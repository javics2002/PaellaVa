#include "Reloj.h"
#include "../../Scenes/GameOver.h"

Reloj::Reloj(Game* game)
	: GameObject(game)
{
	this->game = game;
	setDimension(w, h);
	setPosition(sdlutils().width() - getWidth(), getHeight());
	setActive(true);
	lastUpdate_ = 0;

	relojTexture = &sdlutils().images().at("reloj");

	hourIni = 9;
	minuteIni = 0;
	
	currentTime.hours = hourIni;
	currentTime.minutes = minuteIni;
	string timeText = parseTimeToString(hourIni, minuteIni);

	setTexture(timeText, string("paella"), fgColor, bgColor);
}

Reloj::~Reloj()
{
}

bool Reloj::finDia()
{
	return SDL_TICKS_PASSED(SDL_GetTicks(), totalJornada);
}

void Reloj::render(SDL_Rect* cameraRect)
{

	drawRender(getCollider(), relojTexture);
	drawRender(cameraRect);

}


void Reloj::update()
{

	if (lastUpdate_ + updateTime_ > sdlutils().currRealTime()) { //si no pasan
		return;
	}

	lastUpdate_ = sdlutils().currRealTime();

	if (finDia())
	{
		//TODO: Evento de fin del día
		//game->changeScene(new GameOver(game, 200));

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
