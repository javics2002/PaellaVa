#include "Posicion.h"

Posicion::Posicion()
{
	x = y = 0;
}

Posicion::Posicion(float x)
{
	this->x = y = x;
}

Posicion::Posicion(float x, float y)
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

void Posicion::Set(float x, float y)
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

float Posicion::GetX() const
{
	return x;
}

float Posicion::GetY() const
{
	return y;
}
