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
	posCola = pos;
	clientes = clientes_;
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

	return true;
}
