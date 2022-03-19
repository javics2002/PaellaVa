#pragma once
#include "../GameObject.h"
class Reloj : public GameObject
{

public:
	Reloj(Game* game);
	~Reloj();
	bool finDia();
	void update() override;

private:
	unsigned int tInit = 0, currentHour = 0, hoursTotal = 10000;
	const unsigned int MIN_TICKS = 1000; //ticks en un min (1 hora en el juego)

	struct time { int hours = 0; int minutes = 0; };
	time currentTime;

	int h = 50;
	int w = 120;
};

