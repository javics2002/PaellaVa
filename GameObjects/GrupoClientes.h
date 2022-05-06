#pragma once

#include <vector>
#include <array>
#include <List>;

#include "Cliente.h"
#include "ObjetoPortable.h"

#include "../Data/Comanda.h"
#include "Imagen.h"

class Cola;
class Pedido;
class Game;
class Mesa;

enum EstadoClientes { CAMINANDO, ENCOLA, PIDIENDO, ESPERANDO, COMIENDO, CUENTA };

class GrupoClientes : public ObjetoPortable
{
	const unsigned int DIMENSION = 150;

	vector<Cliente*> clientes;
	std::array<SDL_Rect, 8> pickedClientsPos;

	list<GrupoClientes*>::const_iterator posCola;
	Cola* cola;

	const float DIMIN_TIME = 6000;
	const float DIMIN_TOLERANCIA = 1;
		
	float lastTimeTol;
	float tolerancia;

	vector<string> texturaTolerancia = {"enfadado", "enfadado_2", "basico", "feliz_2", "feliz", "feliz"};

	Texture* texTolerancia;

	Imagen* dolarCuenta;

	Mesa* mesa;

	float lastTimePed;
	
	int itemNow;
	bool showPed;
	bool orderStart;
	bool vertical;
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
	GrupoClientes(Game* mGame);	
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
	void modificaPedido(int numPaellas, std::vector<int> tamPaellas, std::vector<int> ingPedidos);
	void decirPedidio();
	Pedido* getPedido() { return pedido; }

	void setOrientacion(bool vertical_);

	bool paellasPedidas();

	void setGoshtGroup();
};






