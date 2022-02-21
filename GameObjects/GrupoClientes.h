#pragma once

#include <vector>
#include <List>;
#include "Cliente.h"
#include "PoolObject.h"

class Game;

class GrupoClientes : public PoolObject
{
	vector<Cliente*> clientes;
	list<GrupoClientes*>::const_iterator posCola;

	TextureName texturaTolerancia;

	//Comanda* comanda;
	bool espera = false;

	const int dimension = 60;

public:
	GrupoClientes(Game* game) : PoolObject(game), texturaTolerancia(barraTolerancia) {};
	~GrupoClientes() = default;

	void Pedir();
	void Comer();
	float Puntuacion();
	
	void setGrupo(list<GrupoClientes*>::const_iterator pos, vector<Cliente*> clientes_);

	void update() {};

	bool collide(SDL_Rect rect) override;

	float mitadGrupo();

	bool ratonEncima() override;
};

