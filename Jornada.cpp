#include "Jornada.h"
#include <SDL.h>

Jornada::Jornada(Game* game)
{
	tInit = SDL_GetTicks();
	hoursTotal += SDL_GetTicks();
}

bool Jornada::finDia()
{
	return SDL_TICKS_PASSED(SDL_GetTicks(), hoursTotal);
}

void Jornada::update()
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
