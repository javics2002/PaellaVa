#include "Pedido.h"

Pedido::Pedido(int numComensales, int numeroTiles)
{

	comensales = numComensales;
	tilesMesa = numeroTiles;
	ingredientesPedidos = vector<bool>(tipoIngrediente::LAST, false);

	TipoPaella tamano;
	int suma_tamanos = 0;

	tipoIngrediente ingredienteRand;

	if (comensales <= LIMITE_TAMANO) {
		max_rand = comensales;
	}
	else {
		max_rand = LIMITE_TAMANO;
	}
	

	for (int i = 0; i < tilesMesa && !tamanos_escogidos; i++)
	{
		int tamano_elegido;
		if ((0 < comensales <= 3) && (tilesMesa == 1)) {
			int t = comensales;
			tamano = Volumen(t - 1);
			tamano_elegido = t;
			}
		else {
			int t_random = rand() % max_rand + 1;
			tamano = Volumen(t_random-1);
			tamano_elegido = t_random;
		}
		suma_tamanos += tamano_elegido;
		

		//explorar la posibiidad de crear un min rand en funcion del numero de tiles
		//explorar la posibilidad de un while(con el for dentro) en funcion de si tamano escogida
		//y tamanoescogida variarlo fuera del for en un if
		if (suma_tamanos > comensales) {
			suma_tamanos -= tamano_elegido;
			i--;
		}
		else if (suma_tamanos < comensales) {
			pedidoPaella nuevaPaella;
			paellas.push_back(nuevaPaella);
			paellas[i].tamanoPaella = tamano;
		}
		else if (suma_tamanos == comensales) {
			pedidoPaella nuevaPaella;
			paellas.push_back(nuevaPaella);
			paellas[i].tamanoPaella = tamano;
			tamanos_escogidos = true;
		}
	}

	for (int g = 0; g < paellas.size(); g++) {
		int c = rand() % LIMITE_INGR + 0;
		for (int j = 0; j < c; j++) {
			int n = rand() % (tipoIngrediente::LAST);
			ingredienteRand = tipoIngrediente(n);
			if (paellas[g].ingredientesPedido.size() < LIMITE_INGR && !ingredientesPedidos[ingredienteRand]) 
			{
				paellas[g].ingredientesPedido.push_back(ingredienteRand);
				ingredientesPedidos[ingredienteRand] = true;
			}
			else
			{
				j--;
			}
		}
		ingredientesPedidos = vector<bool>(tipoIngrediente::LAST, false);
	}
}

vector<pedidoPaella> Pedido::getPedido()
{
	return paellas;
}
