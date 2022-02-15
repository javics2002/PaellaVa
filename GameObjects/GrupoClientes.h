#pragma once

#include <vector>
#include "Cliente.h"
#include "GameObject.h"

class Game;



class GrupoClientes : public GameObject
{
	vector<Cliente*> *clientes;
	//Comanda* comanda;
	bool espera = false;

public:
	GrupoClientes(Game* game, vector<Cliente*> * clientes_);
	~GrupoClientes() {};
	void Pedir();
	void Comer();
	float Puntuacion();
};

