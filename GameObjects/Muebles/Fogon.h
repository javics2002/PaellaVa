#pragma once
#include "Mueble.h"

class Game;
class Paella;

class Fogon : public Mueble {
private:
	Paella* paella_;
	vector<int> tiemposDeCoccion = { 3000, 6000, 9000, 12000, 15000 };



	const double movimientoFlecha=1;
	const int flechaCoccionX = 20;
	const int flechaCoccionY = 15;

	int barraCoccionX = 65;
	int barraCoccionY = 10;

	int flechaCoccionPosX = getX() - getWidth() / 2;


public:

	Fogon(Game* game, Vector2D<double> pos);
	~Fogon() = default;
	void update() override;
	void render(SDL_Rect* cameraRect)override;
	virtual bool receivePaella(Paella* pa) override;
	virtual bool returnObject(Player* p) override;
};

