#pragma once
#include "Mueble.h"

class Herramienta;

class CajaHerramientas : public Mueble
{
private:
	Herramienta* tool_ = nullptr;

public:
	CajaHerramientas(Game* game, Vector2D<double> pos);
	~CajaHerramientas() = default;

	virtual bool returnObject(Player* p) override;
	virtual bool receiveHerramienta(Herramienta* h) override;
};



