#pragma once
#include "../Silla.h"
class SillaN : public Silla
{
public:
	SillaN(Game* game, Vector2D<double> pos, string texture);
	~SillaN() = default;

	void sentarCliente(Cliente * c) override;
};

