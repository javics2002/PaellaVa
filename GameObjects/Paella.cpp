#include "Paella.h"
#include "../sdlutils/SDLUtils.h"

Paella::Paella(Tamaño tamaño_):ObjetoPortable() , tamaño(tamaño_)
{

	setPosition(sdlutils().width() / 2, 100);
	setDimension(100, 100);

	setTexture("barraTolerancia");

	switch (tamaño)
	{
	case Mediana:
		sumaIntervalo = 5000;
		break;
	case Grande:
		sumaIntervalo = 10000;
		break;
	default:
		break;
	}
}

void Paella::añadeIngr(Ingrediente ingr_)
{
	if (ingredientes.size() < MAX_INGR) ingredientes.push_back(ingr_);
}

void Paella::setState(Estado estado_)
{
	estado = estado_;
	tiempo = sdlutils().currRealTime();
}

void Paella::update()
{
	switch (estado)
	{
	case Preparacion:
		//Setear textura de preparacion
		setState(Coccion);
		break;
	case Coccion:

		//Setear textura dependiendo del estado de la paella

		if (i < tiemposDeCoccion.size()) {

			auto t = sdlutils().currRealTime() - tiempo;

			if (t > tiemposDeCoccion[i] - tiempoCoccion) {
				tiempoCoccion = tiemposDeCoccion[i];
				tiempo = sdlutils().currRealTime();
				i++;
				estadoFinal = Resultado(i);
			}
		}

		else setState(Preparada);

		break;
	case Preparada:

		//Setear textura de preparada

		break;
	}

	cout << "Estado del proceso: " << estado << " Estado paella: " << estadoFinal << endl;
}

Estado Paella::getState()
{
	return estado;
}
