#pragma once

#include "Mueble.h"
#include <deque>


class Paella;
class Player;
class ObjectManager;

class Pila : public Mueble
{

private:
	int miTipo;
	int numPaellas;
	deque<Paella*> paellas;

public:

	Pila(Game* mGame, Vector2D<double> pos, int miTipo_, int numPaellas);
	~Pila() = default;

	virtual bool returnObject(Player * p) override;
	virtual bool receivePaella(Paella * pa) override;

	void init(ObjectManager* objectManager) override;
};


