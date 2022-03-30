#pragma once
#include "Mueble.h"

class Game;
class Paella;

class Fogon : public Mueble {
private:
	Paella* paella_;

public:

	Fogon(Game* game, Vector2D<double> pos);
	~Fogon() = default;
	virtual bool receivePaella(Paella* pa) override;
	virtual bool returnObject(Player* p) override;
};

