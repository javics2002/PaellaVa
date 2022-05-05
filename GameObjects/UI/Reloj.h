#pragma once
#include "../GameObject.h"
#include "../../Utils/Texture.h"

class Reloj : public GameObject
{
	//Funcionamiento del reloj: tiene que actualizarse cada X segundos de la vida real con un 
	//equivalente de Y minutos del juego
public:
	Reloj(Game* game, int numJornada);
	~Reloj();
	bool finDia();
	void render(SDL_Rect* cameraRect) override;
	void update() override;

	virtual void setTime(double newTime) { mLastUpdate = newTime; }
	virtual double getTime() { return mLastUpdate; }

private:
	Game* mGame;
	Texture* mRelojTexture;
	
	double mLastUpdate; //tiempo desde el �ltimo update
	bool mUltimaHora;

	Sint32 mUpdateTime = 10000; // los milisegundos que tarda en actualizarse el reloj

	int mHourIni = 12; //la hora del d�a a la que empieza la jornada
	int mMinuteIni = 0; //los minutos de la hora a la que empieza el juego
	int mHourFin = 18; //la hora del dia a la que termina la jornada
	int mMinuteFin = 0; //los minutos de la hora del dia a la que termina la jornada

	int mAddedMinutes = 10; //los minutos a�adidos en cada update 

	unsigned int tInit = 0;

	const unsigned int MIN_TICKS = 1000; //ticks en un min (1 hora en el juego)

	struct Time { int hours = 0; int minutes = 0; };
	Time mCurrentTime;

	//Text* hourText = new Text(game, string("hola"), string("paella"));

	string parseTimeToString(int hours, int minutes);
	int mHeight = 50;
	int mWidth = 120;

	const SDL_Color& FG_COLOR = { 255, 255, 255, 255 };
	const SDL_Color& BG_COLOR = { 0, 0, 0, 0 };

	int mNumeroJornada;
};

