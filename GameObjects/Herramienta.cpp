#include "Herramienta.h"
#include "../Control/Game.h"


Herramienta::Herramienta(Game* game) : ObjetoPortable(game) {
	setDimension(DIMENSION, DIMENSION);
	setTexture("llaveInglesa");
	setDepth(2);
}

