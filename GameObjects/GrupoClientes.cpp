#include "GrupoClientes.h"

#include "../Control/Game.h"
#include "../Control/ObjectManager.h"

#include "../GameObjects/Cola.h"

GrupoClientes::GrupoClientes(Game* game) : PoolObject(game) 
{
	setDimension(DIMENSION, DIMENSION);

	setTexture("berenjena");

	texTolerancia = &sdlutils().images().at("barraTolerancia");
	showTol = false;
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

	if (showTol) {
		if (!isPicked()) {

			int tolX = 80;
			int tolY = 60;

			SDL_Rect rect = {};

			switch (estado_)
			{
			case CAMINANDO:
			case ENCOLA:
				rect = { mitadGrupo() - tolX / 2,
					(int)clientes[0]->getY() - clientes[0]->getHeight() / 2 - tolY, tolX, tolY };
				break;
			case SENTADO:
			case CUENTA:
				rect = { (int)getX() - tolX / 2, (int)getY() - tolY, tolX, tolY };
				break;
			default:
				break;
			}

			drawRender(cameraRect, rect, texTolerancia);
		}

		showTol = false;
	}

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
	cout << tolerancia << endl;

	showTol = true;

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

int GrupoClientes::mitadGrupo()
{
	int mitad = 0;

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


