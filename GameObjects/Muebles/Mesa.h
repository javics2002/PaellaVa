#pragma once
#include "Mueble.h"
#include "Silla.h"

#include "../../GameObjects/GrupoClientes.h"
#include "../../GameObjects/Player.h"

class Game;

class Mesa : public Mueble
{
	int mWidht, mHight;

	GrupoClientes* mGrupo;

	std::vector<Silla*> sillas;
	int nSillas;

public:
	Mesa(Game* game, Vector2D<double> pos, Vector2D<int> dim, string texture);
	~Mesa() = default;

	void init() override;

	bool recieveGrupoClientes(GrupoClientes* gc) override;
	virtual bool returnObject(Player* p) override;

};

