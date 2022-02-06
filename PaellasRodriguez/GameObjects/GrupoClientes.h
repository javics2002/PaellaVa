#pragma once
#include "ObjetoPortable.h"
#include <vector>
class Cliente;
class Comanda;

using namespace std;
using uint = uint32_t;

class GrupoClientes : public ObjetoPortable
{
	uint numeroClientes;
	vector<Cliente*> clientes;
	Comanda* comanda;
public:
	void Pedir();
	void Comer();
	float Puntuacion();
};

