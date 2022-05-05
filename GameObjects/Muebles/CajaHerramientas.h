#pragma once
#include "Mueble.h"

class Herramienta;

class CajaHerramientas : public Mueble
{
public:
	CajaHerramientas(Game* mGame, Vector2D<double> pos);
	~CajaHerramientas() = default;

	virtual bool returnObject(Player* p) override;
	virtual bool receiveHerramienta(Herramienta* h) override;
};



