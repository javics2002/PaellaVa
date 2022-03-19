#include "Reloj.h"

Reloj::Reloj(Game* game)
	: GameObject(game)
{
	setDimension(w, h);
	setPosition(sdlutils().width() - getWidth(), getHeight() );
	setActive(true);

	int horaa = 12;
	string timeText =  "porfs123";

	setTexture(timeText, string("paella"), fgColor, bgColor);
}

Reloj::~Reloj()
{
}

bool Reloj::finDia()
{
	return SDL_TICKS_PASSED(SDL_GetTicks(), hoursTotal);
}

void Reloj::update()
{

	if (finDia())
	{
		//TODO: Evento de fin del día

	}
	else
	{
		//1 minuto en Ticks = 1 hora en el juego
		currentTime.hours = (SDL_GetTicks() - tInit) / MIN_TICKS;
		currentTime.minutes = (SDL_GetTicks() - tInit) % MIN_TICKS;

		//TODO: Representar cuantas horas han pasado
		//renderTimeText();
		string timeText = to_string(currentTime.hours) + ":" + to_string(currentTime.minutes);

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
