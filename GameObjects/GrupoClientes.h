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

	//Comanda* comanda;
	bool espera = false;

	int x, y;

public:
	GrupoClientes(Game* game) : PoolObject(game) {};
	~GrupoClientes() = default;

	void Pedir();
	void Comer();
	float Puntuacion();
	
	void setGrupo(list<GrupoClientes*>::const_iterator pos, vector<Cliente*> clientes_);

	void update();
};

