#include "Paella.h"
#include "../sdlutils/SDLUtils.h"
#include "Ingrediente.h"
#include "Ingredienteletal.h"
#include "Arroz.h"
#include "../Utils/ParticleExample.h"
#include <map>

Paella::Paella(Game* mGame, int tipo) : ObjetoPortable(mGame), miTipo(tipo)
{
	setDepth(2);

	switch (miTipo)
	{
	case Pequena:
		setDimension(40, 40);
		break;
	case Mediana:
		setDimension(52, 52);
		mSumaIntervalo = 5000;
		break;
	case Grande:
		setDimension(65, 65);
		mSumaIntervalo = 10000;
		break;
	default:
		break;
	}

	setTexture("paellaLimpia");

	mIngrEnPaella = vector<bool>(tipoIngrediente::LAST, false);

	mHumo = new ParticleExample();
	mHumo->setRenderer(sdlutils().renderer());
	mHumo->setStyle(ParticleExample::NONE);

	mCurrentCoccionSound = "paella";
}

void Paella::anadeIngr(Ingrediente* ingr_)
{
	//Si ya he echado arroz
	if (mArroz) {
		//Añadimos el ingrediente
		sdlutils().soundEffects().at("dejarIngrediente2").play(0, mGame->UI);
		if (ingr_->esLetal())
			mContaminada = true;
		mIngredientes.push_back(ingr_->getTipo());
		mIngrEnPaella[ingr_->getTipo()] = true;
		ingr_->setActive(false);
	}
}

void Paella::anadeArroz(Arroz* arroz)
{
	//Si aun no tengo arroz
	if (!mArroz) {
		//Añadimos arroz a la paella
		sdlutils().soundEffects().at("dejarIngrediente2").play(0, mGame->UI);
		mArroz = true;
		mEstadoCoccion = Cruda;
		setContenido(Entera);
		setTexture("paellaCruda");
		arroz->setActive(false);
	}
}

void Paella::setState(EstadoPaellas estado_)
{
	estado = estado_;
	mTiempo = sdlutils().currRealTime();

	//Sonido
	if (estado == Coccion) {
		mCurrentCoccionSound = "paellaCociendo";
		mInitCocTime = sdlutils().virtualTimer().currTime();
		canalSonido = sdlutils().soundEffects().at(mCurrentCoccionSound).play(-1);

	}
	else
		sdlutils().soundEffects().at(mCurrentCoccionSound).haltChannel(canalSonido);
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
		break;
	case Coccion: // Cuando esta en coccion, se va sumando un contador y se quema si llega a cierto tiempo
		if (mEstadoCoccion < mTiemposDeCoccion.size() && sdlutils().virtualTimer().currTime() - mInitCocTime >= mTiemposDeCoccion[mEstadoCoccion]) {
			mEstadoCoccion++;
			setTexture(mCoccionTex[mEstadoCoccion]);

			if (mEstadoCoccion == MuyHecha) {
				sdlutils().soundEffects().at(mCurrentCoccionSound).haltChannel(canalSonido);
				mCurrentCoccionSound = "paella";
				canalSonido = sdlutils().soundEffects().at(mCurrentCoccionSound).play(-1);
			}
			if (mEstadoCoccion == Quemada) {
				mHumo->setStyle(ParticleExample::FIRE);
				sdlutils().soundEffects().at("fuego").play();
			}
			if (mEstadoCoccion == Incomestible) {
				mHumo->setStyle(ParticleExample::SMOKE);
			}
		}
		break;
	case Hecha:
		if (sdlutils().virtualTimer().currTime() - mInitHumoTime >= mTiempoHumo) {
			mHumo->setStyle(ParticleExample::NONE);
		}
		break;
	}

	mHumo->setPosition(getX(), getY());
	mHumo->update();
}

void Paella::finLavado() // Se termina de lavar
{
	mContenido = Limpia;
	estado = Preparacion;
	mContaminada = false;
	setTexture("paellaLimpia");
}

void Paella::iniLavado()
{
	estado = Lavandose;
}

void Paella::onObjectPicked() // Si se estaba cocinando pasa a estar hecha, y si esta hecha y esta quemada te quemas
{
	if (estado == Coccion) {
		setState(Hecha);
	}
	else if(estado == Hecha)
		if (mEstadoCoccion >= Quemada) {
			//humo->setStyle(ParticleExample::SMOKE);
			mInitHumoTime = sdlutils().virtualTimer().currTime();
		}
}

void Paella::onObjectDropped()
{
}

bool Paella::canPick()
{
	return mContenido == Limpia && !mEnMesa;
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
	return mIngredientes;
}
vector<bool> Paella::getIngrPaella()
{
	return mIngrEnPaella;
}

int Paella::getTipo()
{
	return miTipo;
}
int Paella::getContenido()
{
	return mContenido;
}

int Paella::getCoccion()
{
	return mEstadoCoccion;
}

void Paella::setEnsuciada() // Se ensucia la paella al tirarla a la basura
{
	mIngredientes.clear();
	mArroz = false;
	for (int i : mIngrEnPaella) {
		mIngrEnPaella[i] = false;
	}
}

void Paella::setContenido(Contenido contenidoP) // Actualiza el contenido
{
	mContenido = contenidoP;
	// mHumo->setStyle(ParticleExample::NONE);
}

bool Paella::ingrValido(Ingrediente* ingr)
{
	return mIngredientes.size() < MAX_INGR && estado == Cruda && !mIngrEnPaella[ingr->getTipo()];
}

EstadoPaellas Paella::getState()
{
	return estado;
}

void Paella::comerPaella() // Cambia los sprites de la paella segun su estado
{
	mContenido++;

	if (mContenido == Mitad) 
		setTexture(mCoccionTex[mEstadoCoccion] + "Mitad");
	else {
		setTexture("paellaSucia");
		mArroz = false;
	}
}
void Paella::contaminaPaella()
{
	mContaminada = true;
}
bool Paella::estaContaminada()
{
	return mContaminada;
}

int Paella::ingredientesEnPaella() // Devuelve el numero de ingredientes que se han metido en la paella
{
	return mIngredientes.size();
}

void Paella::render(SDL_Rect* cameraRect)
{
	if (estado != Lavandose) { // Si no se esta lavando se renderiza, sino no se renderiza para dar la sensacion de estar dentro del lavavajillas
		drawRender(cameraRect);

		if (mContenido == Entera) { // Se renderiza la paella en funcion de su estado
			for (auto i : mIngredientes) {
				drawRender(cameraRect, getTexBox(), &sdlutils().images().at(texturaIngrediente[i] + "C"));
			}
		}
		else if (mContenido == Mitad) {
			for (auto i : mIngredientes) {
				drawRender(cameraRect, getTexBox(), &sdlutils().images().at(texturaIngrediente[i] + "M"));
			}
		}
		// Se renderiaz el humo en caso de haber
		mHumo->draw(cameraRect);
	}	
}
