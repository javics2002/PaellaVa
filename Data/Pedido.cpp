#include "Pedido.h"
#include "../Scenes/Tutorial.h"
#include "../Control/Game.h"

Pedido::Pedido(Game* game_, int numComensales, int numeroTiles)
{
	game = game_;
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
			tamano = TipoPaella(t - 1);
			tamano_elegido = t;
		}
		else {
			int t_random = rand() % max_rand + 1;
			tamano = TipoPaella(t_random - 1);
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
		int c = rand() % (LIMITE_INGR + 1);
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

Pedido::Pedido(Game* game_, int numPaellas, vector<int> tamPaellas, vector<int> ingPedidos)
{
	game = game_;

	for (int i = 0; i < numPaellas; i++) {
		pedidoPaella nuevaPaella;
		paellas.push_back(nuevaPaella);
		paellas[i].tamanoPaella = tamPaellas[i];

		for (int j = 0; j < ingPedidos.size(); j++) {
			if (ingPedidos[j] == LAST) break;
			paellas[i].ingredientesPedido.push_back(ingPedidos[j]);
		}
	}
	
}

vector<pedidoPaella> Pedido::getPedido()
{
	return paellas;
}

double Pedido::puntuarPedido(vector<Paella*> comanda)
{
	//quitar pedido ->
	penalizacionTamano valorarT;
	penalizacionIngredientes valorarI;
	penalizacionCoccion valorarC;
	
	for (int i = 0; i < getPedido().size(); i++) {

		//Variar puntuacion en funcion de la diferencia de tamaños
		
		int tamanoPaella1 = int(getPedido()[i].tamanoPaella +1);
		int tamanoPaella2 = int(comanda[i]->getTipo() + 1);
		int diferencia = tamanoPaella1 - tamanoPaella2;
		if (comanda[i]->estaContaminada())
		{
			getPedido()[i].puntuacionPaella = 0;
		}
		else {
			switch (diferencia) {
			case 2:
				variarPuntuacion(valorarT.pequenaDos, i);
				break;
			case 1:
				variarPuntuacion(valorarT.pequenaUno, i);
				break;
			case -2:
				variarPuntuacion(valorarT.grandeDos, i);
				break;
			case -1:
				variarPuntuacion(valorarT.grandeUno, i);
				break;
			default:
				break;
			}

			//Variar puntuacion en funcion de la coccion de la paella.

			switch (comanda[i]->getCoccion()) {
			case Cruda:
				variarPuntuacion(valorarC.cruda, i);
				break;
			case PocoHecha:
				variarPuntuacion(valorarC.pocoHecha, i);
				break;
			case Perfecta:
				variarPuntuacion(valorarC.perfecta, i);
				break;
			case MuyHecha:
				variarPuntuacion(valorarC.muyHecha, i);
				break;
			case Quemada:
				variarPuntuacion(valorarC.quemada, i);
				break;
			case Incomestible:
				variarPuntuacion(valorarC.incomestible, i);
				break;
			}

			//Variar puntuaciones en funcion de la diferencia de ingredientes.

			int cantidadIngr1 = int(getPedido()[i].ingredientesPedido.size());;
			int cantidadIngr2 = int(comanda[i]->getVIngredientes().size());
			int difCantidad = cantidadIngr2 - cantidadIngr1;
			variarPuntuacion(valorarI.sobraIngr * abs(difCantidad), i);
		}
		//Variar puntuaciones en funcion de los ingredientes que no se han cocinado.

		for (int j = 0; j < getPedido()[i].ingredientesPedido.size(); j++) {
			if (!comanda[i]->getIngrPaella()[getPedido()[i].ingredientesPedido[j]]) {
				variarPuntuacion(valorarI.faltaIngr, i);
			}
		}
		cout << getPedido()[i].puntuacionPaella << endl;
		if (getPedido()[i].puntuacionPaella > 10) {
			getPedido()[i].puntuacionPaella = 10;
		}
		sumaMedia += getPedido()[i].puntuacionPaella;
	}
	puntuacionPedido = sumaMedia / getPedido().size();

	cout << puntuacionPedido << endl;
	return puntuacionPedido;
}




void Pedido::variarPuntuacion(double variacion, int pos)
{
	double puntos = getPedido()[pos].puntuacionPaella + variacion; 
	paellas[pos].puntuacionPaella = puntos;
}

vector<string> Pedido::getPedidoTex()
{
	vector<string> v;

	if(dynamic_cast<Tutorial*>(game->getCurrentScene())){
		v.push_back(paellasTamTex[0]);
		v.push_back(texturaIngrediente[0]);
		v.push_back(texturaIngrediente[1]);
		v.push_back(texturaIngrediente[2]);
		v.push_back("finPedido");
	}
	else {
		for (auto i : paellas) {
			v.push_back(paellasTamTex[i.tamanoPaella]);
			if (!i.ingredientesPedido.empty()) {
				for (auto j : i.ingredientesPedido) {
					v.push_back(texturaIngrediente[j]);
				}
			}
			else v.push_back("sinIngredientes");
		}
		v.push_back("finPedido");
	}


	return v;
}