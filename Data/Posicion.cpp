#include "Posicion.h"

Posicion::Posicion()
{
	x = y = 0;
}

Posicion::Posicion(int x)
{
	this->x = y = x;
}

Posicion::Posicion(int x, int y)
{
	this->x = x;
	this->y = y;
}

Posicion::Posicion(const Posicion& pos)
{
	this->x = pos.GetX();
	this->y = pos.GetY();
}

Posicion::~Posicion()
{
}

void Posicion::Set(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Posicion::Set(const Posicion& pos)
{
	this->x = pos.GetX();
	this->y = pos.GetY();
}

Posicion& Posicion::Get()
{
	return *this;
}

int Posicion::GetX() const
{
	return x;
}

int Posicion::GetY() const
{
	return y;
}
