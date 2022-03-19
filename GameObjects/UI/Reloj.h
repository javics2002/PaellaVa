#pragma once
#include "../GameObject.h"
#include "../UI/Text.h"

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

	//Text* hourText = new Text(game, string("hola"), string("paella"));

	void renderTimeText();
	int h = 50;
	int w = 120;

	const SDL_Color& fgColor = { 255, 255, 255, 255 };
	const SDL_Color& bgColor = { 0, 0, 0, 0 };
};

