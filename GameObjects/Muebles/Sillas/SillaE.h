#pragma once
#include "../Silla.h"

class Cliente;

class SillaE : public Silla
{
public:
	SillaE(Game* mGame, Vector2D<double> pos, string texture);
	~SillaE() = default;

	void sentarCliente(Cliente* c) override;
};

