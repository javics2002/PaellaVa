#include "GrupoClientes.h"
#include "../Control/Game.h"

void GrupoClientes::Pedir()
{

}

void GrupoClientes::Comer()
{

}

float GrupoClientes::Puntuacion()
{
	return 0.0f;
}


void GrupoClientes::setGrupo(list<GrupoClientes*>::const_iterator pos, vector<Cliente*> clientes_)
{
	tolerancia = 100;

	posCola = pos;
	clientes = clientes_;

	setState(CAMINANDO);
}

void GrupoClientes::update()
{

	if (estado_ == CAMINANDO) {

		for (auto i : clientes) {
			i->update();

		}

		SDL_Rect rect = {clientes[0]->getX() + clientes[0]->getWidth() / 2, clientes[0]->getY() - clientes[0]->getHeight() / 2,
			clientes[0]->getWidth()/2, clientes[0]->getWidth() };

		for (auto i : game->getClientes(rect)) {
			 setState(ENCOLA);
		}
	}

	else if (estado_ == ENCOLA) {
		bajaTolerancia();
	}
}

bool GrupoClientes::collide(SDL_Rect rect)
{
	for (auto i:clientes) {
		if (i->collide(rect)) {
			return true;
		}
	}

	return false;
}

float GrupoClientes::mitadGrupo()
{
	float mitad = 0.0f;

	for (auto i:clientes) {
		mitad += i->getX();
	}

	return mitad/clientes.size();
}

bool GrupoClientes::ratonEncima()
{
	SDL_Rect rect = { mitadGrupo()-dimension/2, clientes[0]->getY() - clientes[0]->getHeight()*1.25, dimension, dimension };

	drawTexture(game->getTexture(texturaTolerancia), rect);

	cout << tolerancia << endl;

	return true;
}

bool GrupoClientes::colisionClientes()
{
	if(estado_ == CAMINANDO)setState(ENCOLA);
	return true;
}

void GrupoClientes::bajaTolerancia()
{

	if (tolerancia > 0 && SDL_GetTicks() - mLastTime >= TIME_BAJADA) {

		tolerancia -= BAJADA;

		if (tolerancia < 0) tolerancia = 0;

		mLastTime = SDL_GetTicks();
	}
}

void GrupoClientes::setState(estado est)
{
	estado_ = est;
	mLastTime = SDL_GetTicks();
}
