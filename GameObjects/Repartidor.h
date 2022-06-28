#pragma once

#include <vector>
#include <array>
#include <List>;

#include "Cliente.h"

#include "../Data/Comanda.h"
#include "Imagen.h"
#include "../Data/PedidoTakeaway.h"
#include "Muebles/Mueble.h"

class ColaTakeaway;
class Pedido;
class Game;
class Mesa;

enum EstadoRepartidor { repCAMINANDO, repENCOLA, repESPERANDO, repSALIENDO };

class Repartidor : public Mueble
{
	const unsigned int DIMENSION = 150;

	// vector<Cliente*> clientes;
	// std::array<SDL_Rect, 8> pickedClientsPos;

	list<Repartidor*>::const_iterator posCola;
	ColaTakeaway* cola;

	const float DIMIN_TIME = 5000;
	const float DIMIN_TOLERANCIA = 1;

	float lastTimeTol;
	float tolerancia;

	vector<string> texturaTolerancia = { "enfadado", "enfadado_2", "basico", "feliz_2", "feliz", "feliz" };

	Texture* texTolerancia;

	float lastTimePed;

	int itemNow;
	bool showPed;
	bool orderStart;
	bool vertical;
	PedidoTakeaway* pedido;
	vector<string> texPedido;
	CajaTakeaway* cajaTakeaway;

	SDL_RendererFlip flip;
	SDL_Rect clip;

	const float PED_DELAY = 2000;
	const float TIEMPO_COMIDA = 5000;

	EstadoRepartidor estado_;

	void bajaTolerancia();
	bool ratonEncima(SDL_Rect* cameraRect);

public:
	Repartidor(Game* mGame);
	~Repartidor();

	void init(ColaTakeaway* cola_);

	void update() override;

	void render(SDL_Rect* cameraRect) override;
	void renderDebug(SDL_Rect* cameraRect) override {};

	bool colisionCartel();

	void setState(EstadoRepartidor est);
	EstadoRepartidor getState();

	void onDeactivate() override;

	void hacerPedido();
	void modificaPedido(std::vector<int> ingPedidos);
	void decirPedidio();
	PedidoTakeaway* getPedido() { return pedido; }

	bool receiveCajaTakeaway(CajaTakeaway* caja) override;

	void setOrientacion(bool vertical_);
	void animPara();
	void animCamina();
};

