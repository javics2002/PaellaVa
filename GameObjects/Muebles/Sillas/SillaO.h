#pragma once

#include "../Silla.h"

class SillaO : public Silla
{
public:
	SillaO(Game* mGame, Vector2D<double> pos, string texture);
	~SillaO() = default;

	void sentarCliente(Cliente * c) override;
};

