#pragma once
#include "Mueble.h"

class CajaTakeaway;

class TorreCajaTakeaway : public Mueble
{
public:
	TorreCajaTakeaway(Game* mGame, Vector2D<double> pos);
	~TorreCajaTakeaway() = default;

	virtual bool returnObject(Player* p) override;
	virtual bool receiveCajaTakeaway(CajaTakeaway* c) override;
};



