#pragma once
#include "../GameObject.h"
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

	virtual void setTime(double newTime) { mLastUpdate = newTime; }
	virtual double getTime() { return mLastUpdate; }

private:
	Game* game;
	Texture* relojTexture;
	
	double mLastUpdate; //tiempo desde el �ltimo update
	bool ultimaHora;

	Uint32 updateTime_ = 5000; // los milisegundos que tarda en actualizarse el reloj

	int hourIni = 12; //la hora del d�a a la que empieza la jornada
	int minuteIni = 0; //los minutos de la hora a la que empieza el juego
	int hourFin = 18; //la hora del dia a la que termina la jornada
	int minuteFin = 0; //los minutos de la hora del dia a la que termina la jornada

	int addedMinutes = 10; //los minutos a�adidos en cada update 

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

