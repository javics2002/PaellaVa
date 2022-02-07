#pragma once

class Game;

class Jornada
{
	unsigned int tInit, currentTime = 0, hoursTotal = 10000;
	const unsigned int MIN_TICKS = 1000;

	Jornada(Game* game);
	bool finDia();
	void update();
};

