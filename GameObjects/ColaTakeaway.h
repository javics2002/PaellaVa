#pragma once

#include "Repartidor.h"
#include <list>

using namespace std;

class ColaTakeaway
{
	int t_Max = 0;
	list<Repartidor*> cola;
	int capacidadActual;

	void colaAvanza();

public:

	ColaTakeaway(int tMax_) : t_Max(tMax_), capacidadActual(0) {};
	~ColaTakeaway() = default;

	list<Repartidor*>::const_iterator getPos();

	void add(Repartidor* grupo);
	void remove(list<Repartidor*>::const_iterator it);

	bool esValido();
	bool estaLlena();

};