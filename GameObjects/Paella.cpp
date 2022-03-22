#include "Paella.h"
#include "../sdlutils/SDLUtils.h"
#include "Ingrediente.h"
#include <map>

Paella::Paella(Game* game, TipoPaella tipo): ObjetoPortable(game), miTipo(tipo)
{

	setPosition(sdlutils().width() / 2, 100);
	setDimension(50, 50);

	switch (miTipo)
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

	setTexture("paella");

	ingrEnPaella = vector<bool>(tipoIngrediente::LAST, false);
}

void Paella::anadeIngr(Ingrediente* ingr_)
{
	ingredientes.push_back(ingr_->getTipo());
	ingrEnPaella[ingr_->getTipo()] = true;
}

void Paella::eliminarIngr()
{
	ingredientes.clear();
	for (int i : ingrEnPaella) {
		ingrEnPaella[i] = false;
	}	
}

void Paella::setState(Estado estado_)
{
	estado = estado_;
	tiempo = sdlutils().currRealTime();
}

void Paella::paellaRecogida()
{
	vel = { 0,0 };
}

void Paella::update()
{
	switch (estado)
	{
	case Preparacion:
		//Setear textura de preparacion
		//setState(Coccion);
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
		contenido = Entera;
		break;
	}
}

void Paella::setLavado(Contenido contenidoPaella,string texturaPaella)
{
	contenido = contenidoPaella;
	setTexture(texturaPaella);
}

void Paella::onObjectPicked()
{
	if(estado == Coccion) setState(Preparada);
}

void Paella::onObjectDropped()
{

}

bool Paella::canPick()
{
	return contenido==Limpia;
}
TipoPaella Paella::getTipo()
{
	return miTipo;
}
Contenido Paella::getContenido()
{
	return contenido;
}
void Paella::changeTexture(string clave)
{
	setTexture(clave);
}

bool Paella::ingrValido(Ingrediente* ingr)
{
	return ingredientes.size() < MAX_INGR && estado == Cruda && !ingrEnPaella[ingr->getTipo()];
}

Estado Paella::getState()
{
	return estado;
}
