#pragma once

#include "GrupoClientes.h"
#include <list>

class Cola
{
	const int T_Max = 10;
	list<GrupoClientes> cola;
	int capacidadActual;

public:

	Cola(): capacidadActual(0) {};
	~Cola();

	bool esValido(int tamGrupo);
	void add(GrupoClientes grupo);
};

