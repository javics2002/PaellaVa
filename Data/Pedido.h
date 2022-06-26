#pragma once
#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/Paella.h"

//Struct que contiene el tamaño y los ingredientes propios de una paella.
struct pedidoPaella {
	int tamanoPaella;
	vector<int> ingredientesPedido;
	double puntuacionCaja = 10;
};

//Diversas penalizaciones que recibirá un pedido si no cumple lo requerido por un cliente.
struct PenalizacionTamano {
	double pequenaUno = -1;
	double pequenaDos = -2;
	double grandeUno = -1;
	double grandeDos = -2;
};
struct PenalizacionIngredientes {
	double sobraIngr = -1;
	double faltaIngr = -1;
};
struct PenalizacionCoccion {
	double cruda = -3;
	double pocoHecha = 0;
	double perfecta = 1;
	double muyHecha = 0;
	double quemada = -3;
	double incomestible = -4;
};

const vector<string> PAELLAS_TAM_TEX{ "small", "medium", "large" };

class Game;

class Pedido 
{
private:
	vector<pedidoPaella> mPaellas;

	Game* mGame;

	int mComensales, mTilesMesa, mMaxRand;

	bool mTamanosEscogidos = false;
	bool mPaellaSuspensa = false;

	//Habrá un máximo de 3 tamaños(pequeña, mediana y grande) y un máximo de tres ingredientes por paella.
	const int LIMITE_TAMANO = 3;
	const int LIMITE_INGR = 3;

	double mSumaMedia = 0;
	double mPuntuacionPedido;

public:
	Pedido(Game* game, int numComensales, int numeroTiles);
	Pedido(Game* game, int numPaellas, vector<int> tamPaellas, vector<int> ingPedidos);
	~Pedido() {};
	vector<pedidoPaella> getPedido();
	vector<string> getPedidoTex();
	double puntuarPedido(vector<Paella*> comanda);
	void variarPuntuacion(double variacion, int pos);
};