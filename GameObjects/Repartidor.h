#pragma once

#include <vector>
#include <array>
#include <List>;

#include "Cliente.h"

#include "../Data/Comanda.h"
#include "Imagen.h"
#include "../Data/PedidoTakeaway.h"

class ColaTakeaway;
class Pedido;
class Game;
class Mesa;

enum EstadoRepartidor { repCAMINANDO, repENCOLA, repESPERANDO, repSALIENDO };

class Repartidor : public GameObject
{
	const unsigned int DIMENSION = 150;

	// vector<Cliente*> clientes;
	// std::array<SDL_Rect, 8> pickedClientsPos;

	list<Repartidor*>::const_iterator posCola;
	ColaTakeaway* cola;

	const float DIMIN_TIME = 6000;
	const float DIMIN_TOLERANCIA = 1;

	float lastTimeTol;
	float tolerancia;

	vector<string> texturaTolerancia = { "enfadado", "enfadado_2", "basico", "feliz_2", "feliz", "feliz" };

	Texture* texTolerancia;

	// Imagen* dolarCuenta;

	// Mesa* mesa;

	float lastTimePed;

	int itemNow;
	bool showPed;
	bool orderStart;
	bool vertical;
	PedidoTakeaway* pedido;
	vector<string> texPedido;
	CajaTakeaway* cajaTakeaway;

	const float PED_DELAY = 2000;

	float lastTimeComido;
	bool comidoMitad;
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

	void hacerPedido(int tamMesa);
	void modificaPedido(int numPaellas, std::vector<int> ingPedidos);
	void decirPedidio();
	PedidoTakeaway* getPedido() { return pedido; }

	void setOrientacion(bool vertical_);
	void para();
	void camina();
};

