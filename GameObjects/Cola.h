#pragma once

#include "GrupoClientes.h"
#include <list>

class Cola
{
	const int T_Max = 10;
	list<GrupoClientes*> cola;
	int capacidadActual;

	void colaAvanza();

public:

	Cola(): capacidadActual(0) {};
	~Cola() = default;

	list<GrupoClientes*>::const_iterator getPos();

	void add(GrupoClientes* grupo, int tam);
	void remove(list<GrupoClientes*>::const_iterator it, int tam);

	bool esValido(int tamGrupo);	
	
};

