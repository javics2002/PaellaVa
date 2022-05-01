#pragma once
#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/Paella.h"

struct pedidoPaella {
	int tamanoPaella;
	vector<int> ingredientesPedido;
	double puntuacionPaella = 10;
};

struct penalizacionTamano {
	double pequenaUno = -1;
	double pequenaDos = -2;
	double grandeUno = -1;
	double grandeDos = -2;
};
struct penalizacionIngredientes {
	double sobraIngr = -1;
	double faltaIngr = -1;
};
struct penalizacionCoccion {
	double cruda = -3;
	double pocoHecha = 0;
	double perfecta = 1;
	double muyHecha = 0;
	double quemada = -3;
	double incomestible = -4;
};

const vector<string> paellasTamTex{ "small", "medium", "large" };

class Game;

class Pedido 
{
public:
	Pedido(Game* game, int numComensales, int numeroTiles);
	Pedido(Game* game, int numPaellas, vector<int> tamPaellas, vector<int> ingPedidos);
	~Pedido() {};
	vector<pedidoPaella> getPedido();
	vector<string> getPedidoTex();
	double puntuarPedido(vector<Paella*> comanda);
	void variarPuntuacion(double variacion, int pos);
	//No se hasta que punto merece la pena crear otro m�todo que sea ejecutado en la constructora.
	//void crearPedido(int numCmensales)

private:
	vector<pedidoPaella> paellas;

	Game* game;

	int comensales, tilesMesa, max_rand;

	bool tamanos_escogidos = false;

	const int LIMITE_TAMANO = 3;
	const int LIMITE_INGR = 3;
	
	double sumaMedia = 0;
	double puntuacionPedido;
};

/*

struct con volumen y vector de ingredientes

vector de struct

vector de booleanos

enums de paella e ingrediente




constructora(num comensales)

int comparar(vector de paellas){puntos}

*/