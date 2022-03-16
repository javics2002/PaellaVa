#pragma once
#include "Mueble.h"
#include "../Paella.h"

class Game;

class Lavavajillas : public Mueble
{
public:

	//um Contenido { Limpia, Entera, Mitad, Sucia };

	const unsigned int DIMENSION_W = 1;
	const unsigned int DIMENSION_H = 1;

	const double TIEMPO_LAVADO = 3000.0;
	const int MAX_PAELLERAS = 5;

	int paellas = 0;
	double tiempo = 0.0;

	
	vector<Paella*> paelleras;

public:
	Lavavajillas(Game* game, Vector2D<double> pos);
	~Lavavajillas() = default;

	//void update();
	void onCollision();
	bool LavavajillasLleno();
	void metePaellera(Paella* paella);
	void sacaPaellera(vector<Paella*>::iterator it);
	void lavando();
};

