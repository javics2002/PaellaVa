#pragma once
class Posicion
{
	int x, y;
public:
	Posicion();
	Posicion(int x);
	Posicion(int x, int y);
	Posicion(const Posicion& pos);
	~Posicion();

	void Set(int x, int y);
	void Set(const Posicion& pos);
	Posicion& Get();
	int GetX() const;
	int GetY() const;

	Posicion EnTiles();
};

