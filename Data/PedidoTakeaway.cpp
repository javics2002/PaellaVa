#include "PedidoTakeaway.h"
#include "../Scenes/Tutorial.h"
#include "../Control/Game.h"
#include "../GameObjects/CajaTakeaway.h"
#include "Pedido.h"

PedidoTakeaway::PedidoTakeaway(Game* game_)
{
	mGame = game_;

	vector<bool> ingredientesPedidos = vector<bool>(tipoIngrediente::LAST, false);
	int suma_tamanos = 0;
	tipoIngrediente ingredienteRand;

	int c = rand() % (LIMITE_INGR + 1);
	for (int j = 0; j < c; j++) {
		int n = rand() % (tipoIngrediente::LAST);
		ingredienteRand = tipoIngrediente(n);
		if (mCaja.ingredientesPedido.size() < LIMITE_INGR && !ingredientesPedidos[ingredienteRand])
		{
			mCaja.ingredientesPedido.push_back(ingredienteRand);
			ingredientesPedidos[ingredienteRand] = true;
		}
		else
		{
			j--;
		}
	}

	while (mCaja.ingredientesPedido.size() < 3)
	{
		mCaja.ingredientesPedido.push_back(LAST);
	}

	ingredientesPedidos = vector<bool>(tipoIngrediente::LAST, false); // ??? no sirve de nada xd
}

PedidoTakeaway::PedidoTakeaway(Game* game_, vector<int> ingPedidos)
{
	mGame = game_;

	int cont = 0;

	for (int j = 0; j < 3; j++) { // 3 ing por pedido
		mCaja.ingredientesPedido.push_back(ingPedidos[cont]);
		cont++;
	}
}

pedidoCaja PedidoTakeaway::getPedido()
{
	return mCaja;
}

double PedidoTakeaway::puntuarPedido(CajaTakeaway* comanda)
{
	//quitar pedido ->
	PenalizacionIngredientes valorarI;
	PenalizacionCoccion valorarC;

	//Variar puntuacion en funcion de la diferencia de tamaños

	if (comanda->getContaminada())
	{
		mPaellaSuspensa = true;
	}
	else {

		//Variar puntuacion en funcion de la coccion de la paella.

		switch (comanda->getCocinada()) {
		case Cruda:
			mPaellaSuspensa = true;
			break;
		case PocoHecha:
			variarPuntuacion(valorarC.pocoHecha);
			break;
		case Perfecta:
			variarPuntuacion(valorarC.perfecta);
			break;
		case MuyHecha:
			variarPuntuacion(valorarC.muyHecha);
			break;
		case Quemada:
			variarPuntuacion(valorarC.quemada);
			break;
		case Incomestible:
			mPaellaSuspensa = true;
			break;
		}

		//Variar puntuaciones en funcion de la diferencia de ingredientes.

		int cantidadIngr1 = 0;
		for (int j = 0; j < mCaja.ingredientesPedido.size(); j++) {
			if (int(mCaja.ingredientesPedido[j]) != 9) {
				cantidadIngr1++;
			}

		}
		int cantidadIngr2 = int(comanda->getIngreds().size());
		int difCantidad = cantidadIngr2 - cantidadIngr1;
		variarPuntuacion(valorarI.sobraIngr * abs(difCantidad));
	}

	//Variar puntuaciones en funcion de los ingredientes que no se han cocinado.

	for (int j = 0; j < mCaja.ingredientesPedido.size(); j++) {
		// if (mCaja.ingredientesPedido[j] != 9 && !comanda->getIngrPaella()[mCaja.ingredientesPedido[j]]) {
		if (comanda->containsIngr((tipoIngrediente)mCaja.ingredientesPedido[j])) {
			variarPuntuacion(valorarI.faltaIngr);
		}
	}

	//Una vez hechas las penalizaciones a la paella se añade a la puntuación media de las paellas entregadas as cada mesa.
	if (mPaellaSuspensa) {
		mCaja.puntuacionPedido = 0;
	}
	if (mCaja.puntuacionPedido > 10) {
		mCaja.puntuacionPedido = 10;
	}

	cout << mCaja.puntuacionPedido << endl;
	return mCaja.puntuacionPedido;
}




void PedidoTakeaway::variarPuntuacion(double variacion)
{
	//Método que se encarga de ir cambiando la puntuación que recibe un pedido en funcion de diversas variables.
	double puntos = getPedido().puntuacionPedido + variacion;
	mCaja.puntuacionPedido = puntos;
}

vector<string> PedidoTakeaway::getPedidoTex()
{
	vector<string> v;

	if (dynamic_cast<Tutorial*>(mGame->getCurrentScene())) {
		v.push_back(texturaIngrediente[0]);
		v.push_back(texturaIngrediente[1]);
		v.push_back(texturaIngrediente[2]);
		v.push_back("finPedido");
	}
	else {
		if (!mCaja.ingredientesPedido.empty()) {
			for (auto j : mCaja.ingredientesPedido) {
				if (j != LAST)
					v.push_back(texturaIngrediente[j]);
			}
		}
		else v.push_back("sinIngredientes");

		v.push_back("finPedido");
	}


	return v;
}