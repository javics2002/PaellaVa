#pragma once

#include "Mueble.h"

#include "../../Data/Pool.h"
#include "../Cola.h"
#include "../ColaTakeaway.h"

class Game;

class Cliente;

class Puerta : public Mueble
{
	int maxTamGrupo = 0;

	const int MAX_SPAWN_TIME = 30000;
	const int MIN_SPAWN_TIME = 17500;
	const int FAIL_SPAWN_TIME = 5000;

	const int MAX_REPARTSPAWN_TIME = 30000;
	const int MIN_REPARTSPAWN_TIME = 18000;
	const int FAIL_REPARTSPAWN_TIME = 3000;

	double spawn_delay = 0;
	double repartSpawn_delay = 0;
	double initTime;

	Cola* cola;
	ColaTakeaway* colaTakeaway;

	Uint8 orientacion;

	bool clientSpawn();
	int numClientSpawn();
	bool repartidorSpawn();

	int muebleIdCount = 0;

public:
	Puerta(Game* mGame, Vector2D<double> pos, int t_Max, int maxTamGrupo_);
	~Puerta() = default;

	void update() override;
	virtual bool receiveGrupoClientes(GrupoClientes* gc);

	SDL_Rect getCollider() override;
	SDL_Rect getOverlap() override;

	Cola* getCola();
	ColaTakeaway* getColaTakeaway();

	void setVel(Vector2D<double> vel_) override;
	

	void setOrientation(Cliente* c);
	void setOrientation(Repartidor* rep);
};









