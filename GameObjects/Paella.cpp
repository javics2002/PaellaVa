#include "Paella.h"
#include "../sdlutils/SDLUtils.h"
#include "Ingrediente.h"
#include "Arroz.h"
#include <map>

Paella::Paella(Game* game, TipoPaella tipo) : ObjetoPortable(game), miTipo(tipo)
{
	setPosition(1500, 200);
	setDimension(50, 50);
	setColliderRect({ (int)getX(), (int)getY(), w, h });

	switch (miTipo)
	{
	case Mediana:
		mSumaIntervalo = 5000;
		break;
	case Grande:
		mSumaIntervalo = 10000;
		break;
	default:
		break;
	}

	setTexture("paellaLimpia");

	ingrEnPaella = vector<bool>(tipoIngrediente::LAST, false);
}

void Paella::anadeIngr(Ingrediente* ingr_)
{
	//Si ya he echado arroz
	if (mArroz) {
		//Añadimos el ingrediente
		ingredientes.push_back(ingr_->getTipo());
		ingrEnPaella[ingr_->getTipo()] = true;
		ingr_->deactivate();
	}
}

void Paella::anadeArroz(Arroz* arroz)
{
	//Si aun no tengo arroz
	if (!mArroz) {
		//Añadimos arroz a la paella
		mArroz = true;
		setContenido(Entera);
		setTexture("paella");
		arroz->deactivate();
	}
}

void Paella::setState(EstadoPaellas estado_)
{
	estado = estado_;
	mTiempo = sdlutils().currRealTime();
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

			auto t = sdlutils().currRealTime() - mTiempo;

			if (t > tiemposDeCoccion[i] - mTiempoCoccion) {
				mTiempoCoccion = tiemposDeCoccion[i];
				mTiempo = sdlutils().currRealTime();
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

void Paella::setLavado(Contenido contenidoPaella, string texturaPaella)
{
	contenido = contenidoPaella;
	setTexture(texturaPaella);
}

void Paella::onObjectPicked()
{
	if (estado == Coccion) setState(Preparada);
}

void Paella::onObjectDropped()
{
}

bool Paella::canPick()
{
	return contenido == Limpia && !mEnMesa;
}

bool Paella::conArroz()
{
	return mArroz;
}

void Paella::enLaMesa(bool estaEnLaMesa)
{
	mEnMesa = estaEnLaMesa;
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
