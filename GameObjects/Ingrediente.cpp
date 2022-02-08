#include "Ingrediente.h"

void Ingrediente::activate()
{
	active = true;
}

void Ingrediente::descativate()
{
	active = false;
}

bool Ingrediente::isActive()
{
	return active;
}