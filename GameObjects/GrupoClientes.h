#pragma once

#include <vector>
#include <List>;
#include "Cliente.h"
#include "PoolObject.h"
#include "ObjetoPortable.h"

class Game;

enum estado { SENTADO, COMIENDO, ENCOLA, CAMINANDO };

class GrupoClientes : public PoolObject
{
	vector<Cliente*> clientes;
	list<GrupoClientes*>::const_iterator posCola;

	string texturaTolerancia = "barraTolerancia";

	const float BAJADA = 2;
	const float TIME_BAJADA = 3000;

	bool espera = false;

	estado estado_;

	const int dimension = 60;

	float tolerancia, mLastTime;

public:
	GrupoClientes(Game* game);
	
	~GrupoClientes() = default;

	float Puntuacion();

	void setGrupo(list<GrupoClientes*>::const_iterator pos, vector<Cliente*> clientes_);

	void update();

	bool collide(SDL_Rect rect) override;

	float mitadGrupo();

	bool ratonEncima() override;

	bool colisionClientes() override;

	void bajaTolerancia();

	void setState(estado est);
	estado getState();

	void render(SDL_Rect* cameraRect) override;

	void onObjectPicked() override;
	void onObjectDropped() override;
};






