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
		currentTime = (SDL_GetTicks() - tInit) / MIN_TICKS;

		//TODO: Representar cuantas horas han pasado
	}
}
