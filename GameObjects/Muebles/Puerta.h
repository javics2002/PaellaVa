#pragma once

#include "Mueble.h"

#include "../../Data/Pool.h"
#include "../Cola.h"

class Game;

class Cliente;

class Puerta : public Mueble
{
	int maxTamGrupo = 0;

	const double SPAWN_DELAY = 8000;
	double initTime;
	bool vertical;

	Cola* cola;

	Uint8 orientacion;

public:
	Puerta(Game* game, Vector2D<double> pos, int t_Max, int maxTamGrupo_);
	~Puerta() = default;

	void update() override;
	virtual bool receiveGrupoClientes(GrupoClientes* gc);

	SDL_Rect getCollider() override;
	SDL_Rect getOverlap() override;

	Cola* getCola();

	void setVel(Vector2D<double> vel_) override;
	

	void setOrientation(Cliente* c);
};









