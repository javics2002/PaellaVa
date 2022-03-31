#pragma once
#include "Mueble.h"

class Game;

class BolsaArroz : public Mueble
{
public:
	BolsaArroz(Game* game, Vector2D<double> pos);
	~BolsaArroz() = default;
	
	virtual bool returnObject(Player* p) override;
};

