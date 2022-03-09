#include "Paella.h"
#include "../sdlutils/SDLUtils.h"

Paella::Paella(Tamaño tamaño_):ObjetoPortable() , tamaño(tamaño_)
{
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
}

void Paella::update()
{
	switch (estado)
	{
	case Preparacion:
		break;
	case Coccion:
		tiempoCocion += sdlutils().currRealTime();
		break;
	case Preparada:
		bool encontrado = false;
		int i = 0;
		while (!encontrado && i<tiemposDeCoccion.size()) {
			if (tiempoCocion < tiemposDeCoccion[i] + sumaIntervalo) {
				estadoFinal = Resultado(i);
				encontrado = true;
			}
			i++;
		}
		if (!encontrado)estadoFinal = Incomestible;

		break;
	}
}

Estado Paella::getState()
{
	return estado;
}
