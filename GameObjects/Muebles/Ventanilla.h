#pragma once
#include "Mueble.h"
class Ventanilla : public Mueble
{
	Paella* p = nullptr;
public:
	Ventanilla(Game* game, Vector2D<double> pos);
	bool receivePaella(Paella* pa)override;//return false para no dejar la paella (sin comanda seleccionada no se puede)
	~Ventanilla() = default;
};

