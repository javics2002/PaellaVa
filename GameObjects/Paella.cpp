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
		estadoCoccion = Cruda;
		setContenido(Entera);
		setTexture("paellaCruda");
		arroz->deactivate();
	}
}

void Paella::setState(EstadoPaellas estado_)
{
	estado = estado_;
	mTiempo = sdlutils().currRealTime();

	//Sonido
	if (estado == Coccion) {
		initCocTime = sdlutils().virtualTimer().currTime();
		canalSonido = sdlutils().soundEffects().at("paella").play(-1);
	}	
	else
		sdlutils().soundEffects().at("paella").haltChannel(canalSonido);
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
		break;
	case Coccion:
		if (estadoCoccion < tiemposDeCoccion.size() && sdlutils().virtualTimer().currTime() - initCocTime >= tiemposDeCoccion[estadoCoccion]) {

			estadoCoccion++;
			cout << estadoCoccion << endl;
			setTexture(coccionTex[estadoCoccion]);		
		}
		break;
	case Hecha:
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
	if (estado == Coccion) setState(Hecha);
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

TipoPaella Paella::getTipo()
{
	return miTipo;
}
int Paella::getContenido()
{
	return contenido;
}

int Paella::getCoccoin()
{
	return estadoCoccion;
}

void Paella::setEnsuciada()
{
	ingredientes.clear();
	mArroz = false;
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

void Paella::comerPaella()
{
	contenido++;

	if (contenido == Mitad) 
		setTexture(coccionTex[estadoCoccion] + "Media");
	else setTexture("paellaSucia");
}

void Paella::render(SDL_Rect* cameraRect)
{
	drawRender(cameraRect);


	if (contenido == Entera) {
		for (auto i : ingredientes) {
			drawRender(cameraRect, getTexCollider(), &sdlutils().images().at(texturaIngrediente[i] + "C"));
		}
	}
	else if (contenido == Mitad) {
		for (auto i : ingredientes) {
			drawRender(cameraRect, getTexCollider(), &sdlutils().images().at(texturaIngrediente[i] + "M"));
		}
	}
	
}
