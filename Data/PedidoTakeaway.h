#pragma once
#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/Paella.h"

//Struct que contiene el tamaño y los ingredientes propios de una paella.
struct pedidoCaja {
	vector<int> ingredientesPedido;
	double puntuacionPedido = 10;
};


class Game;
class CajaTakeaway;

class PedidoTakeaway
{
private:
	pedidoCaja mCaja;

	Game* mGame;

	bool mPaellaSuspensa = false;

	// Máximo tres ingredientes por paella.
	const int LIMITE_INGR = 3;
	double mPuntuacionPedido;

public:
	PedidoTakeaway(Game* game);
	PedidoTakeaway(Game* game, vector<int> ingPedidos);
	~PedidoTakeaway() {};
	pedidoCaja getPedido();
	vector<string> getPedidoTex();
	double puntuarPedido(CajaTakeaway* comanda);
	void variarPuntuacion(double variacion);
};