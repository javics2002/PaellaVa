#pragma once

#include "Mueble.h"
#include <deque>

class Paella;


class Pila : public Mueble
{

private:

	//TipoPaella miTipo;

	deque<Paella*> paellas;

public:
	Pila(Game* game, Vector2D<double> pos);
	~Pila() = default;

	virtual bool returnObject(Player * p) override;
	virtual bool receivePaella(Paella * pa) override;
};


