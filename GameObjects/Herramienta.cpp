#include "Herramienta.h"
#include "../Control/Game.h"


Herramienta::Herramienta(Game* mGame) : ObjetoPortable(mGame) {
	setDimension(DIMENSION, DIMENSION);
	setTexture("llaveInglesa");
	setDepth(2);
}

