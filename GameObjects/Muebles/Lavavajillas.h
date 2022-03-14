#pragma once
#include "Mueble.h"
#include "../Paella.h"

class Game;

class Lavavajillas : public Mueble
{
public:

	const unsigned int DIMENSION_W = 1;
	const unsigned int DIMENSION_H = 1;
	
	const int MAX_PAELLERAS = 5;
	int paelleras = 0;
	vector<Paella> paelleras;

public:
	Lavavajillas(Game* game, Vector2D<double> pos);
	~Lavavajillas() = default;

	//void update();
	void onCollision();
	bool LavavajillasLleno();
	void metePaellera();
	void sacaPaellera();
};

