#include "Pedido.h"

Pedido::Pedido(int numComensales, int numeroTiles)
{

	comensales = numComensales;
	tilesMesa = numeroTiles;

	Volumen tamano;
	int suma_tamanos = 0;

	tipoIngrediente ingredienteRand;

	if (comensales <= LIMITE_TAMANO) {
		max_rand = comensales;
	}
	else {
		max_rand = LIMITE_TAMANO;
	}

	for (int i = 0; i <= tilesMesa && !tamanos_escogidos; i++)
	{
		int t = rand() % max_rand + 1;

		tamano = Volumen(t);

		suma_tamanos += t;
		if (suma_tamanos > comensales) {
			suma_tamanos -= t;
			i--;
		}
		else if (suma_tamanos < comensales) {
			paellas[i].tamanoPaella = tamano;
		}
		else if (suma_tamanos == comensales) {
			paellas[i].tamanoPaella = tamano;
			tamanos_escogidos = true;
		}
	}

	for (int g = 0; g <= paellas.size(); g++) {
		for (int j = 0; j < LIMITE_INGR; j++) {
			int n = rand() % (tipoIngrediente::LAST);
			ingredienteRand = tipoIngrediente(n);
			if (paellas[g].ingredientesPedido.size() < LIMITE_INGR && !ingredientesPedidos[ingredienteRand]) {
				paellas[g].ingredientesPedido.push_back(ingredienteRand);
				ingredientesPedidos[ingredienteRand] = true;
			}
			else
			{
				j--;
			}
		}
	}
}
