#include "Herramienta.h"

#include <iostream>

Herramienta::Herramienta(Game* game) : ObjetoPortable(game) {
	setDimension(DIMENSION, DIMENSION);

	setDepth(2);
}

void Herramienta::update()
{

}

void Herramienta::onActivate()
{
	setTexture("berenjena");
}

void Herramienta::onDeactivate()
{
}

void Herramienta::onObjectPicked()
{

}

void Herramienta::onObjectDropped()
{
}
