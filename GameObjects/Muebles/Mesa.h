#pragma once
#include "Mueble.h"
#include "Silla.h"

class Game;

class Mesa : public Mueble
{
	int mWidht_, mHight_;

	std::vector<Silla*> sillas;
	int nSillas;

public:
	Mesa(Game* game, Vector2D<double> pos, Vector2D<int> dim, string texture);
	~Mesa() = default;

	void init() override;

	bool recieveGrupoClientes(GrupoClientes* gc) override;
};

