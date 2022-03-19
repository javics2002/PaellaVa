#include "Reloj.h"

Reloj::Reloj(Game* game)
	: GameObject(game)
{
	setDimension(w, h);
	setPosition(sdlutils().width() - getWidth(), getHeight() );
	setTexture("reloj");
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
	}
}
