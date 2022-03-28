#pragma once
#include "Mueble.h"
#include "../../Data/Pool.h"
#include <deque>

class Game;
class Arroz;

class BolsaArroz : public Mueble
{

	Arroz* arroz = nullptr;

public:
	BolsaArroz(Game* game, Vector2D<double> pos);
	~BolsaArroz() = default;
	
	virtual bool returnObject(Player* p) override;
};

