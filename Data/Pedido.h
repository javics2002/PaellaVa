#pragma once
#include "../GameObjects/Ingrediente.h"
#include "../GameObjects/Paella.h"

struct pedidoPaella {
	TipoPaella tamanoPaella;
	vector<tipoIngrediente> ingredientesPedido;
};

struct penalizacionTamano {
	double pequenaUno = -1;
	double pequenaDos = -2;
	double grandeUno = -0.5;
	double grandeDos = -1;
};
struct penalizacionIngredientes {
	double sobraIngr = -0.1;
	double faltaIngr = -0.25;
};
struct penalizacionCoccion {
	double cruda = -0.5;
	double pocoHecha = 0;
	double perfecta = 0.5;
	double muyHecha = 0;
	double quemada = -0.5;
	double incomestible = -4;
};

class Pedido 
{
public:
	Pedido(int numComensales, int numeroTiles);
	~Pedido() {};
	vector<pedidoPaella> getPedido();
	vector<string> getPedidoTex();
	void puntuarPedido(vector<Paella*> comanda, Pedido* pedido);
	void variarPuntuacion(int variacion);
	//No se hasta que punto merece la pena crear otro método que sea ejecutado en la constructora.
	//void crearPedido(int numCmensales)

private:
	vector<pedidoPaella> paellas;
	vector<bool> ingredientesPedidos;

	vector<string> paellasTex { "arroz", "alcachofa", "calamar", "cangrejo", "gamba",
	"langosta", "mejillon", "pimientoRojo", "pimientoVerde", "pollo" };
	vector<string> paellasTamTex { "small", "medium", "large" };

	int comensales, tilesMesa, max_rand;

	bool tamanos_escogidos = false;

	const int LIMITE_TAMANO = 3;
	const int LIMITE_INGR = 3;
	
	int puntuacion = 10;
};

/*

struct con volumen y vector de ingredientes

vector de struct

vector de booleanos

enums de paella e ingrediente




constructora(num comensales)

int comparar(vector de paellas){puntos}

*/