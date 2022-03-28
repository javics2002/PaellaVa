#pragma once
#include "Mueble.h"
#include "../Paella.h"
#include <deque>

class Game;

class Lavavajillas : public Mueble
{
public:

	//um Contenido { Limpia, Entera, Mitad, Sucia };

	const unsigned int DIMENSION_W = 1;
	const unsigned int DIMENSION_H = 1;

	const double TIEMPO_LAVADO = 3000.0;


	deque<Paella*> pilaPaellas;
	deque<Paella*> paellasLimpias;

public:
	Lavavajillas(Game* game, Vector2D<double> pos);
	~Lavavajillas() = default;

	void update() override;
	void lavando();

	bool receivePaella(Paella* paella_) override;
	bool returnObject(Player* p)override; 
};



