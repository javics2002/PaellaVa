#pragma once
class Posicion
{
	float x, y;
public:
	Posicion();
	Posicion(float x);
	Posicion(float x, float y);
	Posicion(const Posicion& pos);
	~Posicion();

	void Set(float x, float y);
	void Set(const Posicion& pos);
	Posicion& Get();
	float GetX() const;
	float GetY() const;

	Posicion EnTiles();
};

