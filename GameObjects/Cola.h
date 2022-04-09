#pragma once

#include "GrupoClientes.h"
#include <list>

class Cola
{
	int t_Max = 0;
	list<GrupoClientes*> cola;
	int capacidadActual;

	void colaAvanza();

public:

	Cola(int tMax_): t_Max(tMax_), capacidadActual(0) {};
	~Cola() = default;

	list<GrupoClientes*>::const_iterator getPos();

	void add(GrupoClientes* grupo, int tam);
	void remove(list<GrupoClientes*>::const_iterator it, int tam);

	bool esValido(int tamGrupo);	
	
};

