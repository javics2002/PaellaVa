#include "Reloj.h"
#include "../../Scenes/GameOver.h"

Reloj::Reloj(Game* game)
	: GameObject(game)
{
	this->game = game;
	setDimension(w, h);
	setPosition(sdlutils().width() - getWidth(), getHeight() );
	setActive(true);
	lastUpdate_ = 0;
	
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

void Reloj::update()
{

	if (lastUpdate_ + updateTime_ > sdlutils().currRealTime()) { //si no pasan
		return;
	}

	lastUpdate_ = sdlutils().currRealTime();

	if (finDia())
	{
		//TODO: Evento de fin del día
		//game->changeScene(new GameOver(game));

	}
	else
	{
		//1 minuto en Ticks = 1 hora en el juego
		currentTime.hours = (SDL_GetTicks() - tInit) / MIN_TICKS;
		currentTime.minutes = (SDL_GetTicks() - tInit) % MIN_TICKS;

		//TODO: Representar cuantas horas han pasado
		//renderTimeText();
		string timeText = parseTimeToString(currentTime.hours, currentTime.minutes);

		setTexture(timeText, string("paella"), fgColor, bgColor);

	}
}

void Reloj::renderTimeText()
{
	string timeText = currentTime.hours + ":" + currentTime.minutes;
	Texture scoreTex(sdlutils().renderer(), timeText,
		sdlutils().fonts().at("ARIAL24"), build_sdlcolor(0x111111ff));

	SDL_Rect dest = build_sdlrect( //
		getPosition().getX(), //
		getPosition().getY(), //
		scoreTex.width(), //
		scoreTex.height());
	cout << "entra a render time" << endl;
	scoreTex.render(dest);
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
