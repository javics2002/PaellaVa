#pragma once
#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/Paella.h"

struct pedidoPaella {
	TipoPaella tamanoPaella;
	vector<tipoIngrediente> ingredientesPedido;
};

class Pedido 
{
public:
	Pedido(int numComensales, int numeroTiles);
	~Pedido() {};

	//No se hasta que punto merece la pena crear otro método que sea ejecutado en la constructora.
	//void crearPedido(int numCmensales);

private:
	vector<pedidoPaella> paellas;
	vector<bool> ingredientesPedidos;

	int comensales, tilesMesa, max_rand;

	bool tamanos_escogidos = false;

	const int LIMITE_TAMANO = 3;
	const int LIMITE_INGR = 3;
	vector<pedidoPaella> getPedido();

};

/*

struct con volumen y vector de ingredientes

vector de struct

vector de booleanos

enums de paella e ingrediente




constructora(num comensales)

int comparar(vector de paellas){puntos}

*/