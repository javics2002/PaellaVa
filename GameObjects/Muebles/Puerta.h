#pragma once

#include "Mueble.h"

#include "../../Data/Pool.h"
#include "../Cola.h"

class Game;

class Puerta : public Mueble
{
	const unsigned int MAX_TAM = 3;

	const double SPAWN_DELAY = 8000;
	double initTime;
	bool vertical;

	Cola* cola;

	vector<string> texturasClientes = { "clienteBase"};

public:
	Puerta(Game* game, Vector2D<double> pos,bool vertical_,int t_Max);
	~Puerta() = default;

	void update() override;
	virtual bool receiveGrupoClientes(GrupoClientes* gc);

	SDL_Rect getCollider() override;
	SDL_Rect getOverlap() override;
};









