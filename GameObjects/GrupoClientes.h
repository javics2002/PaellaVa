#pragma once

#include <vector>
#include <List>;
#include "Cliente.h"
#include "PoolObject.h"
#include "ObjetoPortable.h"

class Cola;
class Game;

enum estado { CAMINANDO, ENCOLA, SENTADO, CUENTA };

class GrupoClientes : public PoolObject
{
	const unsigned int DIMENSION = 60;

	vector<Cliente*> clientes;

	list<GrupoClientes*>::const_iterator posCola;
	Cola* cola;

	const float DIMIN_TIME = 3000;
	const float DIMIN_TOLERANCIA = 2;
		
	float lastTime;
	float tolerancia;
	bool showTol;

	Texture* texTolerancia;

	estado estado_;

	int mitadGrupo();

	void bajaTolerancia();

public:
	GrupoClientes(Game* game);	
	~GrupoClientes() = default;

	void initGrupo(Cola* cola_, vector<Cliente*> clientes_);

	void update() override;

	void render(SDL_Rect* cameraRect) override;

	bool collide(SDL_Rect rect) override;
	bool colisionClientes() override;

	void setState(estado est);
	estado getState();

	int numIntegrantes();
	vector<Cliente*> getIntegrantes();

	bool ratonEncima() override;
	
	void onObjectPicked() override;
	void onObjectDropped() override;

	bool canPick() override;

	void onDesactivate() override;


};






