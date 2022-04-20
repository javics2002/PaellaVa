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
	double initTime;

	const int MAX_BREAK_TIME = 3000;
	const int REDUCE_BREAK_TIME = 500;
	int couldBreak = MAX_BREAK_TIME;
	int seg = 10;

	deque<Paella*> pilaPaellas;
	deque<Paella*> paellasLimpias;

	int i = 0;

	SDL_Rect clip;

public:
	Lavavajillas(Game* game, Vector2D<double> pos);
	~Lavavajillas() = default;

	void update() override;
	void ensuciarPaellas();
	void lavando();

	bool receivePaella(Paella* paella_) override;
	bool returnObject(Player* p)override; 

	void render(SDL_Rect* camera) override;

	SDL_Rect getOverlap() override;
	SDL_Rect getCollider() override;
};



