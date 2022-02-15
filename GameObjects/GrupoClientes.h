#pragma once

#include <vector>
#include <List>;
#include "Cliente.h"
#include "GameObject.h"

class Game;

class GrupoClientes : public GameObject
{
	vector<Cliente*> *clientes;
	list<GrupoClientes*>::const_iterator posCola;

	//Comanda* comanda;
	bool espera = false;


public:
	GrupoClientes(Game* game, vector<Cliente*> * clientes_);
	~GrupoClientes() {};
	void Pedir();
	void Comer();
	float Puntuacion();

	void setPosCola(list<GrupoClientes*>::const_iterator pos);

	void update() {};
};

