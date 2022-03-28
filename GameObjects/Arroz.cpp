#include "Arroz.h"

Arroz::Arroz(Game* game,string claveTextura) : Ingrediente(game)
{
	setDimension(DIMENSION, DIMENSION);
	setTexture(claveTextura);
}

void Arroz::update()
{
	setPosition(getX(), getY() + vel.getY());
}

void Arroz::onActivate()
{

}

void Arroz::onDesactivate()
{
}


void Arroz::onObjectPicked()
{
	setTexture("alcachofa");
	vel = { 0,0 };
}

void Arroz::onObjectDropped()
{
}
