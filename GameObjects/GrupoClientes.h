#pragma once

#include <vector>
#include <List>;
#include "Cliente.h"
#include "PoolObject.h"

class Game;

enum estado { SENTADO, COMIENDO, ENCOLA, COGIDO, CAMINANDO };

class GrupoClientes : public PoolObject
{
	vector<Cliente*> clientes;
	list<GrupoClientes*>::const_iterator posCola;

	TextureName texturaTolerancia;

	const float BAJADA = 2;
	const float TIME_BAJADA = 3000;
	//Comanda* comanda;
	bool espera = false;

	estado estado_;

	const int dimension = 60;

	float tolerancia, mLastTime;

public:
	GrupoClientes(Game* game) : PoolObject(game), texturaTolerancia(barraTolerancia) {};
	~GrupoClientes() = default;

	void Pedir();
	void Comer();
	float Puntuacion();
	
	void setGrupo(list<GrupoClientes*>::const_iterator pos, vector<Cliente*> clientes_);

	void update();

	bool collide(SDL_Rect rect) override;

	float mitadGrupo();

	bool ratonEncima() override;

	bool colisionClientes() override;

	void bajaTolerancia();

	void setState(estado est);
};

