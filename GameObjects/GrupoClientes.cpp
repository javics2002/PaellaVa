#include "GrupoClientes.h"

#include "../Control/Game.h"
#include "../Control/ObjectManager.h"

#include "../GameObjects/Cola.h"

GrupoClientes::GrupoClientes(Game* game) : PoolObject(game) 
{
	setDimension(DIMENSION, DIMENSION);

	setTexture("berenjena");
}

void GrupoClientes::initGrupo(Cola* cola_, vector<Cliente*> clientes_)
{
	tolerancia = 100;

	posCola = cola_->getPos();
	cola = cola_;
	clientes = clientes_;

	setState(CAMINANDO);
}

void GrupoClientes::update()
{

	if (estado_ == CAMINANDO) {

		for (auto i : clientes) {
			i->update();
		}		
	}

	else if (estado_ == ENCOLA) {
		int n = clientes.size() - 1;

		SDL_Rect rect = { clientes[n]->getX() - clientes[n]->getWidth(), clientes[n]->getY() - clientes[n]->getHeight() / 2,
			clientes[n]->getWidth() / 2, clientes[0]->getWidth() };

		for (auto i : game->getObjectManager()->getPoolGrupoClientes()->getCollisions(rect)) {
			i->colisionClientes();
		}

		bajaTolerancia();
	}
}

void GrupoClientes::render(SDL_Rect* cameraRect)
{
	for (auto i : clientes)
		i->render(cameraRect);

	if (isPicked())
		drawRender(cameraRect);
}

bool GrupoClientes::collide(SDL_Rect rect)
{
	for (auto i : clientes) {
		if (i->collide(rect)) {
			return true;
		}
	}

	return false;
}

bool GrupoClientes::colisionClientes()
{
	if (estado_ == CAMINANDO) 
		setState(ENCOLA);
	return true;
}

bool GrupoClientes::ratonEncima()
{
	SDL_Rect rect = { mitadGrupo() - DIMENSION / 2, clientes[0]->getY() - clientes[0]->getHeight() * 1.25, DIMENSION, DIMENSION };

	Texture *t = &sdlutils().images().at(texturaTolerancia);

	t->render(rect);

	cout << tolerancia << endl;

	return true;
}

void GrupoClientes::bajaTolerancia()
{
	if (tolerancia > 0 && SDL_GetTicks() - lastTime >= DIMIN_TIME) {
		tolerancia -= DIMIN_TOLERANCIA;

		if (tolerancia < 0) tolerancia = 0;

		lastTime = SDL_GetTicks();
	}
}

void GrupoClientes::setState(estado est)
{
	estado_ = est;
	lastTime = SDL_GetTicks();
}

estado GrupoClientes::getState()
{
	return estado_;
}

int GrupoClientes::numIntegrantes()
{
	return clientes.size();
}

vector<Cliente*> GrupoClientes::getIntegrantes()
{
	return clientes;
}



void GrupoClientes::onObjectDropped()
{
	switch (estado_)
	{
	case CAMINANDO:
		break;
	case ENCOLA:
		estado_ = SENTADO;
		cola->remove(posCola, clientes.size());
		break;
	case SENTADO:
		break;
	case CUENTA:
		break;
	default:
		break;
	}
}

void GrupoClientes::onObjectPicked()
{
	switch (estado_)
	{
	case CAMINANDO:
		break;
	case ENCOLA:
		break;
	case SENTADO:
		break;
	case CUENTA:
		break;
	default:
		break;
	}
}

bool GrupoClientes::canPick()
{
	return estado_ == ENCOLA;
}

float GrupoClientes::mitadGrupo()
{
	float mitad = 0.0f;

	for (auto i : clientes) {
		mitad += i->getX();
	}

	return mitad / clientes.size();
}

void GrupoClientes::onDesactivate()
{
	auto list = game->getObjectManager()->getPoolClientes();

	for (auto i : clientes) {
		i->desactivate();
		list->remove(i->getIterator());
	}	
}


