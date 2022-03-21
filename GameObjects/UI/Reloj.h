#pragma once
#include "../GameObject.h"
#include "../UI/Text.h"
#include "../Fondo.h"
#include "../../Utils/Texture.h"

class Reloj : public GameObject
{
	//Funcionamiento del reloj: tiene que actualizarse cada X segundos de la vida real con un 
	//equivalente de Y minutos del juego
public:
	Reloj(Game* game);
	~Reloj();
	bool finDia();
	void render(SDL_Rect* cameraRect) override;
	void update() override;

private:
	Game* game;
	Texture* relojTexture;

	Uint32 lastUpdate_;//tiempo desde el último update
	Uint32 updateTime_ = 1 * 1000; //los segundos que tarda en actualizarse el reloj
	int totalJornada = 5 * 1000 *60; //tiempo real en ms que dura una jornada


	int hourIni = 9; //la hora del día a la que empieza la jornada
	int minuteIni = 0; //los minutos de la hora a la que empieza el juego

	int addedMinutes = 10; //los minutos añadidos en cada update 

	unsigned int tInit = 0;

	const unsigned int MIN_TICKS = 1000; //ticks en un min (1 hora en el juego)

	struct time { int hours = 0; int minutes = 0; };
	time currentTime;

	//Text* hourText = new Text(game, string("hola"), string("paella"));

	string parseTimeToString(int hours, int minutes);
	int h = 50;
	int w = 120;

	const SDL_Color& fgColor = { 255, 255, 255, 255 };
	const SDL_Color& bgColor = { 0, 0, 0, 0 };
};

