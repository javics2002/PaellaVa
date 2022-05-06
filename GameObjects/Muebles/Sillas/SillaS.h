#pragma once
#include "../Silla.h"

class SillaS : public Silla
{
public:
	SillaS(Game* mGame, Vector2D<double> pos, string texture);
	~SillaS() = default;

	void sentarCliente(Cliente * c) override;
};

