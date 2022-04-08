#pragma once
#include "../Scenes/Scene.h"

#include "./tmxlite/Map.hpp"
#include "./tmxlite/Layer.hpp"
#include "./tmxlite/TileLayer.hpp"
#include "./tmxlite/ObjectGroup.hpp"
#include "./tmxlite/Tileset.hpp"

class Game;

class Jornada
{
	unsigned int tInit, currentHour = 0, hoursTotal = 10000;
	const unsigned int MIN_TICKS = 1000; //ticks en un min (1 hora en el juego)

	struct time { int hours = 0; int minutes = 0; };
	time currentTime;

	Jornada(Game* game);
	bool finDia();
	void update();
};

