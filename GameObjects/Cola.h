#pragma once

#include "GrupoClientes.h"
#include <list>

class Cola
{
	const int T_Max = 10;
	list<GrupoClientes*> cola;
	int capacidadActual;

public:

	Cola(): capacidadActual(0) {};
	~Cola() = default;

	bool esValido(int tamGrupo);
	void add(GrupoClientes* grupo, int tam);
	list<GrupoClientes*>::const_iterator getPos();
};

