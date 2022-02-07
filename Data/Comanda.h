#pragma once
#include <vector>

class Paella;
class Ingrediente;

using namespace std;
using uint = uint32_t;

class Comanda
{
public:
	enum Tamaño { Pequeña, Mediana, Grande };
	
	//Compara las paellas recibidas con el pedido y le pone la puntuación
	float Puntuacion(vector<Paella*> paellas);
private:
	uint numeroMesa;
	uint numeroPaellas;

	//Por cada paella, un tamaño y un vector de ingredientes
	vector<Tamaño> tamaños;
	vector<vector<Ingrediente*>> ingredientes;
};

