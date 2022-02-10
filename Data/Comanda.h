#pragma once
#include <vector>
#include "../View/Texture.h"
#include "../SDL2-2.0.20/Nuklear-master/nuklear.h"
#include "../UiButton.h"
#define NK_IMPLEMENTATION
class Paella;
class Ingrediente;

using namespace std;
using uint = uint32_t;
using Iconos = char;
class Comanda
{

public:
	enum Tamaño { Pequeña, Mediana, Grande };
	Comanda(uint nmesa);//crear comanda
	~Comanda();
	void añadirIcono(Iconos i);
	void randomizaIconos();
	void abrirComanda(int x,int y);
	void dibujaPedido();
	void cierraComanda();
	
	//Compara las paellas recibidas con el pedido y le pone la puntuación
	//float Puntuacion(vector<Paella*> paellas);
private:
	int x;
	int y;
	int w;
	int h;
	uint numeroMesa;
	uint numeroPaellas;
	float puntuacion;
	Texture textura;
	//Por cada paella, un tamaño y un vector de ingredientes
	//vector<Tamaño> tamaños;
	//vector<vector<Ingrediente*>> ingredientes;
	vector<Paella> paellas;//datos de la paella recogida en la comanda con la que se calcula la puntuacion
	vector<Iconos> Pedido;//lo que mostramos en la ui  de lo que vas añadiendo a la comanda
	vector<vector< Iconos>> teclado;
	
	float calculaPuntuacion(Paella paella);
	
};

