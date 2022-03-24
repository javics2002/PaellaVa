#pragma once
#include "Mueble.h"
#include "../../Data/Pool.h"
#include <deque>

class Game;
class Ingrediente;

class BolsaArroz : public Mueble
{
private:
	Pool<Ingrediente>* arroces;

public:
	BolsaArroz(Game* game, Vector2D<double> pos);
	~BolsaArroz() = default;
	
	virtual bool returnObject(Player* p) override;
};

