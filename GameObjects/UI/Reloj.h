#pragma once
#include "../GameObject.h"
#include "../UI/Text.h"
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
	virtual void setOffset(double newOffset) { offsetTime = newOffset; }
	virtual void setTime(double newTime) { timeR = newTime; }
	virtual double getTime() { return timeR; }

private:
	Game* game;
	Texture* relojTexture;
	double offsetTime;
	double timeR;

	Uint32 lastUpdate_;//tiempo desde el último update
	Uint32 updateTime_ = 5000; // los milisegundos que tarda en actualizarse el reloj

	int hourIni = 9; //la hora del día a la que empieza la jornada
	int minuteIni = 0; //los minutos de la hora a la que empieza el juego
	int hourFin = 17; //la hora del dia a la que termina la jornada
	int minuteFin = 10; //los minutos de la hora del dia a la que termina la jornada

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

