#pragma once
#include "Mueble.h"

class game;

class Silla : public Mueble
{
	const unsigned int DIMENSION_W = 1;
	const unsigned int DIMENSION_H = 1;

	Mesa* mMesa;
public:
	Silla(Game* game, Vector2D<double> pos, string texture);
	~Silla() = default;

	Silla* initMesa(Mesa* mesa) override;

	bool receiveGrupoClientes(GrupoClientes* gc) override;
	bool receivePaella(Paella* paella) override;
	virtual bool returnObject(Player* p) override;

	SDL_Rect getOverlap() override;
	SDL_Rect getCollider() override;

	void decirPedido() override;
};

