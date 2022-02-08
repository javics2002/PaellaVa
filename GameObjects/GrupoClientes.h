#pragma once
#include "ObjetoPortable.h"
#include <vector>
#include "Cliente.h"

class Comanda;

using namespace std;
using uint = uint32_t;

class GrupoClientes : public ObjetoPortable
{
	uint numeroClientes;
	vector<Cliente*> clientes;
	Comanda* comanda;
	bool espera = false;
	uint paciencia;
	int integrantes = 0;

public:
	GrupoClientes();
	void ini(int numClientes);
	void Pedir();
	void Comer();
	float Puntuacion();
	void NuevoGrupo();
};

