#include "Paella.h"
#include "../sdlutils/SDLUtils.h"
#include "Ingrediente.h"
#include <map>

Paella::Paella(Game* game, TipoPaella tipo): ObjetoPortable(game), miTipo(tipo)
{
	setPosition(1500, 200);
	setDimension(50, 50);
	setColliderRect({ (int)getX(), (int)getY(), w, h });

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

	setTexture("paellaLimpia");

	ingrEnPaella = vector<bool>(tipoIngrediente::LAST, false);
}

void Paella::anadeIngr(Ingrediente* ingr_,bool arroz)
{

	if (!arroz && arroz_) {
		ingredientes.push_back(ingr_->getTipo());
		ingrEnPaella[ingr_->getTipo()] = true;
		ingr_->setPosition(getPosition());
	}
	else if(!arroz_ && arroz){
		arroz_ = true;
		setContenido(Entera);
		game->getObjectManager()->arrozColocado();
		setTexture("paella");
	}

}


void Paella::setState(EstadoPaellas estado_)
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
	setColliderRect({ (int)getX(), (int)getY(), w, h });
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
	return contenido==Limpia && !enMesa;
}

bool Paella::conArroz()
{
	return arroz_;
}

void Paella::enLaMesa(bool estaEnLaMesa)
{
	enMesa = estaEnLaMesa;
}

list<tipoIngrediente> Paella::getVIngredientes()
{
	return ingredientes;
}
vector<bool> Paella::getIngrPaella()
{
	return ingrEnPaella;
}
Resultado Paella::getResultado()
{
	return estadoFinal;
}
TipoPaella Paella::getTipo()
{
	return miTipo;
}
Contenido Paella::getContenido()
{
	return contenido;
}
void Paella::setEnsuciada()
{
	ingredientes.clear();
	for (int i : ingrEnPaella) {
		ingrEnPaella[i] = false;
	}
}

void Paella::setContenido(Contenido contenidoP)
{
	contenido = contenidoP;
}

bool Paella::ingrValido(Ingrediente* ingr)
{
	return ingredientes.size() < MAX_INGR && estado == Cruda && !ingrEnPaella[ingr->getTipo()];
}

EstadoPaellas Paella::getState()
{
	return estado;
}
