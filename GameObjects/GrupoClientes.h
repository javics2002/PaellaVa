#pragma once

#include <vector>
#include <List>;

#include "Cliente.h"
#include "ObjetoPortable.h"

#include "../Data/Comanda.h"

class Cola;
class Pedido;
class Game;
class Mesa;

enum EstadoClientes { CAMINANDO, ENCOLA, PIDIENDO, ESPERANDO, COMIENDO, CUENTA };

class GrupoClientes : public ObjetoPortable
{
	const unsigned int DIMENSION = 60;

	vector<Cliente*> clientes;

	list<GrupoClientes*>::const_iterator posCola;
	Cola* cola;

	const float DIMIN_TIME = 3000;
	const float DIMIN_TOLERANCIA = 2;
		
	float lastTimeTol;
	float tolerancia;

	vector<string> texturaTolerancia = {"enfadado", "enfadado_2", "basico", "feliz_2", "feliz", "feliz"};

	Texture* texTolerancia;

	Mesa* mesa;

	float lastTimePed;
	
	int itemNow;
	bool showPed;
	bool orderStart;
	Pedido* pedido;
	vector<string> texPedido;
	
	const float PED_DELAY = 2000;

	float lastTimeComido;
	bool comidoMitad;
	const float TIEMPO_COMIDA = 5000;

	EstadoClientes estado_;

	int nPaellas;

	int mitadGrupo();
	void bajaTolerancia();

	bool ratonEncima(SDL_Rect* cameraRect);

public:
	GrupoClientes(Game* game);	
	~GrupoClientes();

	void initGrupo(Cola* cola_, vector<Cliente*> clientes_);

	void update() override;

	void render(SDL_Rect* cameraRect) override;
	void renderDebug(SDL_Rect* cameraRect) override {};

	bool overlap(SDL_Rect rect) override;
	bool collide(SDL_Rect rect) override;
	bool colisionClientes();

	void setState(EstadoClientes est);
	EstadoClientes getState();

	int numIntegrantes();
	vector<Cliente*> getIntegrantes();
	
	void onObjectPicked() override;
	void onObjectDropped() override;

	bool canPick() override;
	bool canDrop() override;

	void onDeactivate() override;

	void hacerPedido(int tamMesa, Mesa* m);
	void decirPedidio();

	bool paellasPedidas();
};






