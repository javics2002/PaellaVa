#include "Pedido.h"
#include "../Scenes/Tutorial.h"
#include "../Control/Game.h"

Pedido::Pedido(Game* game_, int numComensales, int numeroTiles)
{
	mGame = game_;
	mComensales = numComensales;
	mTilesMesa = numeroTiles;

	vector<bool> ingredientesPedidos = vector<bool>(tipoIngrediente::LAST, false);
	TipoPaella tamano;
	int suma_tamanos = 0;
	tipoIngrediente ingredienteRand;

	//Se determina los tamaños que se podrán elegir para las diferentes paellas.
	if (mComensales <= LIMITE_TAMANO) {
		mMaxRand = mComensales;
	}
	else {
		mMaxRand = LIMITE_TAMANO;
	}
	//El numero de tiles determina la cantidad de paellas como máximo que se podrán servir en una mesa, pero se puede alimentar a todos los comensales con menos paellas.
	for (int i = 0; i < mTilesMesa && !mTamanosEscogidos; i++)
	{
		int tamanoElegido;
		if ((0 < mComensales <= 3) && (mTilesMesa == 1)) {
			int t = mComensales;
			tamano = TipoPaella(t - 1);
			tamanoElegido = t;
		}
		else {
			int t_random = rand() % mMaxRand + 1;
			tamano = TipoPaella(t_random - 1);
			tamanoElegido = t_random;
		}
		suma_tamanos += tamanoElegido;

		if (suma_tamanos > mComensales) {
			suma_tamanos -= tamanoElegido;
			i--;
		}
		else if (suma_tamanos < mComensales) {
			pedidoPaella nuevaPaella;
			mPaellas.push_back(nuevaPaella);
			mPaellas[i].tamanoPaella = tamano;
		}
		else if (suma_tamanos == mComensales) {
			pedidoPaella nuevaPaella;
			mPaellas.push_back(nuevaPaella);
			mPaellas[i].tamanoPaella = tamano;
			mTamanosEscogidos = true;
		}
	}
	for (int g = 0; g < mPaellas.size(); g++) {
		int c = rand() % (LIMITE_INGR + 1);
		for (int j = 0; j < c; j++) {
			int n = rand() % (tipoIngrediente::LAST);
			ingredienteRand = tipoIngrediente(n);
			if (mPaellas[g].ingredientesPedido.size() < LIMITE_INGR && !ingredientesPedidos[ingredienteRand])
			{
				mPaellas[g].ingredientesPedido.push_back(ingredienteRand);
				ingredientesPedidos[ingredienteRand] = true;
			}
			else
			{
				j--;
			}
		}

		while (mPaellas[g].ingredientesPedido.size() < 3)
		{
			mPaellas[g].ingredientesPedido.push_back(LAST);
		}

		ingredientesPedidos = vector<bool>(tipoIngrediente::LAST, false); // ??? no sirve de nada xd
	}
}

Pedido::Pedido(Game* game_, int numPaellas, vector<int> tamPaellas, vector<int> ingPedidos)
{
	mGame = game_;

	int cont = 0;

	for (int i = 0; i < numPaellas; i++) {
		pedidoPaella nuevaPaella;
		mPaellas.push_back(nuevaPaella);
		mPaellas[i].tamanoPaella = tamPaellas[i];

		for (int j = 0; j < 3; j++) { // 3 ing por pedido
			mPaellas[i].ingredientesPedido.push_back(ingPedidos[cont]);
			cont++;
		}
	}
	
}

vector<pedidoPaella> Pedido::getPedido()
{
	return mPaellas;
}

double Pedido::puntuarPedido(vector<Paella*> comanda)
{
	//quitar pedido ->
	PenalizacionTamano valorarT;
	PenalizacionIngredientes valorarI;
	PenalizacionCoccion valorarC;
	
	for (int i = 0; i < mPaellas.size(); i++) {

		//Variar puntuacion en funcion de la diferencia de tamaños
		
		int tamanoPaella1 = int(mPaellas[i].tamanoPaella +1);
		int tamanoPaella2 = int(comanda[i]->getTipo() + 1);
		int diferencia = tamanoPaella1 - tamanoPaella2;
		if (comanda[i]->estaContaminada())
		{
			mPaellaSuspensa = true;
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
				mPaellaSuspensa = true;
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
				mPaellaSuspensa = true;
				break;
			}

			//Variar puntuaciones en funcion de la diferencia de ingredientes.

			int cantidadIngr1 = 0;
			for (int j = 0; j < mPaellas[i].ingredientesPedido.size(); j++) {
				if (int(mPaellas[i].ingredientesPedido[j]) != 9) {
					cantidadIngr1++;
				}
				
			}
			int cantidadIngr2 = int(comanda[i]->getVIngredientes().size());
			int difCantidad = cantidadIngr2 - cantidadIngr1;
			variarPuntuacion(valorarI.sobraIngr * abs(difCantidad), i);
		}
		//Variar puntuaciones en funcion de los ingredientes que no se han cocinado.

		for (int j = 0; j < mPaellas[i].ingredientesPedido.size(); j++) {
			if (mPaellas[i].ingredientesPedido[j] != 9  && !comanda[i]->getIngrPaella()[mPaellas[i].ingredientesPedido[j]]) {
				variarPuntuacion(valorarI.faltaIngr, i);
			}
		}

		if (mPaellaSuspensa) {
			mPaellas[i].puntuacionPaella = 0;
		}
		cout << mPaellas[i].puntuacionPaella << endl;
		if (mPaellas[i].puntuacionPaella > 10) {
			mPaellas[i].puntuacionPaella = 10;
		}
		mSumaMedia += getPedido()[i].puntuacionPaella;
	}
	mPuntuacionPedido = mSumaMedia / getPedido().size();

	cout << mPuntuacionPedido << endl;
	return mPuntuacionPedido;
}




void Pedido::variarPuntuacion(double variacion, int pos)
{
	//Método que se encarga de ir cambiando la puntuación que recibe un pedido en funcion de diversas variables.
	double puntos = getPedido()[pos].puntuacionPaella + variacion; 
	mPaellas[pos].puntuacionPaella = puntos;
}

vector<string> Pedido::getPedidoTex()
{
	vector<string> v;

	if(dynamic_cast<Tutorial*>(mGame->getCurrentScene())){
		v.push_back(PAELLAS_TAM_TEX[0]);
		v.push_back(texturaIngrediente[0]);
		v.push_back(texturaIngrediente[1]);
		v.push_back(texturaIngrediente[2]);
		v.push_back("finPedido");
	}
	else {
		for (auto i : mPaellas) {
			v.push_back(PAELLAS_TAM_TEX[i.tamanoPaella]);
			if (!i.ingredientesPedido.empty()) {
				for (auto j : i.ingredientesPedido) {
					if(j != LAST)
						v.push_back(texturaIngrediente[j]);
				}
			}
			else v.push_back("sinIngredientes");
		}
		v.push_back("finPedido");
	}


	return v;
}