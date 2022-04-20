#pragma once
#include "Mueble.h"

class Game;
class Paella;

class Fogon : public Mueble {
private:
	Paella* paella_;
	float tiempoDeCoccion = 15000;

	const int MAX_BREAK_TIME = 5000;
	const int REDUCE_BREAK_TIME = 500;
	int couldBreak = MAX_BREAK_TIME;
	int seg = 10;

	const int flechaCoccionX = 15;
	const int flechaCoccionY = 15;

	int barraCoccionX = 120;
	int barraCoccionY = 15;

	bool barra = false;

	SDL_Rect dest_1 = { (int)getX() -barraCoccionX/2 + flechaCoccionX/2,(int) getY() - (int) (getHeight()/ 1.5) - (int)(barraCoccionY /1.5),
	flechaCoccionX, flechaCoccionY };

public:
	Fogon(Game* game, Vector2D<double> pos);
	~Fogon() = default;
	void render(SDL_Rect* cameraRect)override;
	void update();
	virtual bool receivePaella(Paella* pa) override;
	virtual bool returnObject(Player* p) override;
};

